.code

; hi sir

PUBLIC ntreadvirtualmemory
ntreadvirtualmemory PROC
	mov r10, rcx
	mov eax, 63
	syscall
	ret
ntreadvirtualmemory ENDP

PUBLIC ntwritevirtualmemory
ntwritevirtualmemory PROC
	mov r10, rcx
	mov eax, 58
	syscall
	ret
ntwritevirtualmemory ENDP

PUBLIC ntallocatevirtualmemory
ntallocatevirtualmemory PROC
	mov r10, rcx
	mov eax, 24
	syscall
	ret
ntallocatevirtualmemory ENDP

PUBLIC ntprotectvirtualmemory
ntprotectvirtualmemory PROC
	mov r10, rcx
	mov eax, 80
	syscall
	ret
ntprotectvirtualmemory ENDP

END