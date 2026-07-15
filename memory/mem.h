#pragma once
#include <windows.h>
#include <cstdint>
#include <tlhelp32.h>
#include <string>
#include <psapi.h>
#include <vector>

#include "curl/curl.h"

extern "C" {
    uintptr_t ntreadvirtualmemory(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
    uintptr_t ntwritevirtualmemory(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
    uintptr_t ntallocatevirtualmemory(HANDLE, PVOID*, ULONG_PTR, PSIZE_T, ULONG, ULONG);
    uintptr_t ntprotectvirtualmemory(HANDLE, PVOID*, PSIZE_T, ULONG, PULONG);
}

namespace mem {
    inline DWORD rbx_pid = 0;
    inline HANDLE rbx_handle = nullptr;
    inline uintptr_t rbx_base = 0;

    inline DWORD get_pid(const std::string& name) {
        DWORD pid = 0;
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);
        const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (!snap || snap == INVALID_HANDLE_VALUE) return pid;
        if (!Process32First(snap, &pe)) return pid;
        do {
            if (_strcmpi(pe.szExeFile, name.c_str()) == 0) {
                pid = pe.th32ProcessID;
                break;
            }
        } while (Process32Next(snap, &pe));
        CloseHandle(snap);
        rbx_pid = pid;
        return pid;
    }

    inline HANDLE get_handle() {
        auto rbx = OpenProcess(
            PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION,
            FALSE,
            rbx_pid
        );
        rbx_handle = rbx;
        return rbx;
    }

    inline uintptr_t get_base() {
        uintptr_t base = 0;
        MODULEENTRY32 me;
        me.dwSize = sizeof(MODULEENTRY32);
        const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, rbx_pid);
        if (!snap || snap == INVALID_HANDLE_VALUE) return base;
        if (Module32First(snap, &me)) base = reinterpret_cast<uintptr_t>(me.modBaseAddr);
        CloseHandle(snap);
        rbx_base = base;
        return base;
    }

    inline size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
        size_t realsize = size * nmemb;
        ((std::string*)userp)->append((char*)contents, realsize);
        return realsize;
    }

    inline std::string http_get(std::string url) {
        std::string response = "";
        CURL* curl = curl_easy_init();
        if (!curl) return "";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        auto http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        curl_easy_cleanup(curl);
        if (res != CURLE_OK || http_code != 200) return "";
        return response;
    }

    inline std::string get_roblox_product_version() {
        char file_path[MAX_PATH];
        if (!GetModuleFileNameEx(rbx_handle, 0, file_path, MAX_PATH)) return "";
        DWORD e;
        auto sz = GetFileVersionInfoSize(file_path, &e);
        if (!sz) return "";
        std::vector<BYTE> buffer(sz);
        if (!GetFileVersionInfo(file_path, 0, sz, &buffer[0])) return "";
        struct LANGANDCODEPAGE {
            WORD wLanguage;
            WORD wCodePage;
        } *lp_translate;
        UINT cb_translate;
        if (!VerQueryValueA(&buffer[0], "\\VarFileInfo\\Translation", (LPVOID*)&lp_translate, &cb_translate)) return "";
        char sub_block[64];
        sprintf_s(sub_block, "\\StringFileInfo\\%04x%04x\\ProductVersion", lp_translate[0].wLanguage, lp_translate[0].wCodePage);
        char* product_version = nullptr;
        UINT len = 0;
        if (!VerQueryValueA(&buffer[0], sub_block, (LPVOID*)&product_version, &len)) return "";
        return std::string(product_version);
    }

    inline std::string get_roblox_version() {
		auto product_version = get_roblox_product_version();
        auto url = "https://setup.rbxcdn.com/DeployHistory.txt";
        auto deployment_history = http_get(url);
        if (deployment_history.empty()) {
            printf("[-] [get_roblox_version] failed to get deployment history.");
            return "";
        }

        // product version is like x, xxx, x, xxxxxxx,
        // deployment history lines are like
        // New WindowsPlayer version-xxxxxxxxxxxxxxxx at 0/00/0000 00:00:00 XM, file version: x, xxx, x, xxxxxxx, git hash: x.xxx.x.xxxxxxx ...

        std::string target = "file version: " + product_version;
        size_t pos = deployment_history.rfind(target);
        while (pos != std::string::npos) {
            size_t line_start = deployment_history.rfind('\n', pos);
            line_start = (line_start == std::string::npos) ? 0 : line_start + 1;
            std::string line = deployment_history.substr(line_start, pos - line_start);
            if (line.find("New WindowsPlayer") != std::string::npos) {
                size_t hash_start = line.find("version-");
                if (hash_start != std::string::npos) {
                    size_t hash_end = line.find(' ', hash_start);
                    return line.substr(hash_start, hash_end - hash_start);
                }
            }
            if (line_start == 0) break;
            pos = deployment_history.rfind(target, line_start - 1);
        }
        printf("[-] [get_roblox_version] failed to get version string for product version %s", product_version.c_str());
        return "";
    }
}

template <typename T>
inline T read(const uintptr_t addr) {
    T buffer{};
    SIZE_T bytes_read;
    auto res = ntreadvirtualmemory(mem::rbx_handle, (void*)(addr), (void*)(&buffer), sizeof(T), &bytes_read);
    if (res != 0 || bytes_read != sizeof(T)) {
        printf("[-] read failed @ 0x%llx ( status - 0x%llx )\n", addr, res);
        return T();
    }
    return buffer;
}

template <typename T>
inline bool write(const uintptr_t addr, T value) {
    SIZE_T bytes_written;
    auto res = ntwritevirtualmemory(mem::rbx_handle, (void*)(addr), (void*)(&value), sizeof(T), &bytes_written);
    if (res != 0 || bytes_written != sizeof(T)) {
        printf("[-] write failed @ 0x%llx ( status - 0x%llx )\n", addr, res);
        return false;
    }
    return true;
}

inline std::string read_string(uintptr_t addr) {
    auto len = read<uint64_t>(addr + 0x10);
    if (len > 15) addr = read<uint64_t>(addr);
    std::string str; str.reserve((size_t)(len));
    for (size_t i = 0; i < (size_t)(len); ++i) { auto ch = read<char>(addr + i); if (ch == '\0') break; str.push_back(ch); }
    return str;
}

// all stuff below is for write string :sob:

//union rbx_string_data {
//    uint8_t raw[16];
//    uint64_t pointer;
//};
//
//struct rbx_string {
//    rbx_string_data data;
//    uint64_t length;
//    uint64_t capacity;
//};
//
//inline uintptr_t allocate(size_t size) {
//	uintptr_t base = 0;
//    auto res = ntallocatevirtualmemory(mem::rbx_handle, (PVOID*)(&base), 0, &size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
//    if (res != 0)
//        printf("[-] allocation failed ( status - 0x%llx, size - %llu )\n", res, size);
//    printf("[+] allocated memory at 0x%llX ( size - %llu )\n", base, size);
//    return base;
//}
//
//inline bool write_bytes(const uintptr_t addr, std::string str) {
//    SIZE_T bytes_written;
//    auto res = ntwritevirtualmemory(mem::rbx_handle, reinterpret_cast<PVOID>(addr), (void*)(str.c_str()), str.length(), &bytes_written);
//    if (res != 0 || bytes_written != str.length()) {
//        printf("[-] write_bytes failed @ 0x%llx ( status - 0x%x )\n", addr, res);
//        return false;
//    }
//    return true;
//}
//
//inline bool write_string(uintptr_t addr, std::string new_str) {
//    printf("a");
//	auto str = read<rbx_string>(addr);
//    printf("b");
//    if ((uint64_t)(new_str.length()) > str.capacity) {
//        printf("c");
//        while ((uint64_t)(new_str.length()) > str.capacity) {
//            printf("d");
//            str.capacity *= 2;
//            str.capacity += 1;
//		}
//        printf("e");
//        str.data.pointer = allocate((size_t)(str.capacity));
//    }
//    printf("f");
//    str.length = (uint64_t)(new_str.length());
//    printf("g");
//    if (str.length > 15) {
//        printf("h");
//		if (!write<rbx_string>(addr, str)) return false;
//        printf("i");
//		addr = str.data.pointer;
//    }
//    else {
//        printf("j");
//		str.capacity = 15;
//        printf("k");
//		if (!write<rbx_string>(addr, str)) return false;
//    }
//    printf("l\n");
//    if (!write_bytes(addr, new_str)) return false;
//    if (!write<char>(addr + str.length, '\0')) {
//        printf("[-] failed to null-terminate string @ 0x%llx\n", addr);
//        return false;
//    }
//    return true;
//}

/*
inline bool write_bytes(const uintptr_t addr, std::string str) {
    SIZE_T bytes_written;
    auto res = ntwritevirtualmemory(mem::rbx_handle, (void*)(addr), (void*)(str.c_str()), str.length(), &bytes_written);
    if (res != 0 || bytes_written != str.length()) {
        printf("[-] write_bytes failed @ 0x%llx ( status - 0x%llx )\n", addr, res);
        return false;
    }
    return true;
}

inline bool write_string(uintptr_t addr, std::string str) {
    auto new_len = (int64_t)(str.length());
    if (!write<int64_t>(addr + 0x10, new_len)) return false;
    addr = new_len > 15 ? read<uintptr_t>(addr) : addr;
    if (!write_bytes(addr, str)) return false;
    return true;
}
*/

// FINAL REWRITE FOR WRITESTRING YEAH!!!!!!!
// - me remembering after 10 mil years (17/12/25)
// these include helpers and shit, may or may not be for writestring but wtv ok tuff

inline uintptr_t allocate(size_t size) {
    PVOID allocated = nullptr;
    auto result = ntallocatevirtualmemory(mem::rbx_handle, &allocated, 0, &size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (result != 0) {
        printf("[-] allocation failed ( err code 0x%llx )\n", result);
        return 0;
    }
    return (uintptr_t)(allocated);
}

inline bool protect(uintptr_t addr, size_t size, ULONG protection) {
    auto base = (PVOID)(addr);
    ULONG old;
    auto result = ntprotectvirtualmemory(mem::rbx_handle, &base, &size, protection, &old);
    if (result != 0) {
        printf("[-] protection change failed @ 0x%llx\n", addr);
        return false;
    }
    return true;
}

inline bool protect(uintptr_t addr, size_t size, ULONG protection, PULONG old) {
    auto base = (PVOID)(addr);
    auto result = ntprotectvirtualmemory(mem::rbx_handle, &base, &size, protection, old);
    if (result != 0) {
        printf("[-] protection change failed @ 0x%llx\n", addr);
        return false;
    }
    return true;
}

inline bool write_bytes(const uintptr_t addr, std::string str) {
    SIZE_T bytes_written;
    auto res = ntwritevirtualmemory(mem::rbx_handle, (void*)(addr), (void*)(str.c_str()), str.length(), &bytes_written);
    if (res != 0 || bytes_written != str.length()) {
        printf("[-] write_bytes failed @ 0x%llx ( status - 0x%llx )\n", addr, res);
        return false;
    }
    return true;
}

// using these cuz i dont wanna fucking read every offset
union rbx_string_data {
    uint8_t raw[16];
    uintptr_t pointer;
};

struct rbx_string {
    rbx_string_data data;
    uintptr_t length;
    uintptr_t capacity;
};

inline bool write_string(uintptr_t addr, std::string new_str) {
    auto str = read<rbx_string>(addr);
    if (new_str.length() > str.capacity) {
        while (new_str.length() > str.capacity) {
            str.capacity *= 2;
            str.capacity += 1;
        }
        str.data.pointer = allocate((size_t)(str.capacity));
    }
    str.length = (uintptr_t)(new_str.length());
    if (str.length > 15) {
        if (!write<rbx_string>(addr, str)) return false;
        addr = str.data.pointer;
    }
    else {
        str.capacity = 15;
        if (!write<rbx_string>(addr, str)) return false;
    }
    if (!write_bytes(addr, new_str)) return false;
    return true;
}