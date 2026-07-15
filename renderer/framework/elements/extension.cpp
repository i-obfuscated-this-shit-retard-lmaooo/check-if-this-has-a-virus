#include "../settings/functions.h"

bool extension_button(const char* desc_id, bool* opened) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(desc_id);

    const ImRect rect(window->DC.CursorPos, window->DC.CursorPos + elements->widgets.extension_size);
    ItemSize(rect, 0.0f);
    if (!ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(rect, id, &hovered, &held); 
    if (pressed) *opened = !*opened;

    draw->fade_rect_filled(window->DrawList, rect.Min + ImVec2(2, 2), rect.Max - ImVec2(2, 2), draw->get_clr(clr->window.background_two), draw->get_clr(clr->window.background_one), fade_direction::vertically);
    draw->rect(window->DrawList, rect.Min, rect.Max, draw->get_clr(var->window.hover_hightlight && hovered ? clr->accent : clr->widgets.stroke_two));
    draw->rect(window->DrawList, rect.Min + ImVec2(1, 1), rect.Max - ImVec2(1, 1), draw->get_clr(clr->window.stroke));
    draw->text_clipped_outline(window->DrawList, var->font.icons[1], rect.Min + ImVec2(1, 0), rect.Max - ImVec2(5, 0), draw->get_clr(clr->widgets.text), *opened ? "B" : "A", NULL, NULL, ImVec2(1.f, 0.5f));
    return pressed;
}

bool c_gui::begin_extension(std::string_view label, ImVec2 size) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    BeginGroup();
    PushID(label.data());
    ImGuiID id = GetID(label.data());
    bool* opened = GetStateStorage()->GetBoolRef(id, false);

    gui->push_style_var(ImGuiStyleVar_PopupBorderSize, 1.f);
    gui->push_style_color(ImGuiCol_Border, draw->get_clr(clr->window.stroke));
    gui->push_style_color(ImGuiCol_PopupBg, draw->get_clr(clr->window.background_one));

    if (extension_button("##ExtensionButton", opened))
    {
        OpenPopup("##ExtensionPopup");
        SetNextWindowPos(g.LastItemData.Rect.GetBL() + ImVec2(0.0f, style.ItemSpacing.y));
        SetNextWindowSize(size);
    }
    
    if (*opened && BeginPopup("##ExtensionPopup"))
    {
        gui->push_style_var(ImGuiStyleVar_WindowPadding, elements->widgets.padding);
        gui->push_style_var(ImGuiStyleVar_ItemSpacing, elements->widgets.spacing);
        auto popup_window = GetCurrentWindow();
        auto popup_pos = popup_window->Pos;
        auto popup_size = popup_window->Size;
        auto draw_list = popup_window->DrawList;
        auto cursor_pos = popup_window->DC.CursorPos;
        draw->rect_filled(draw_list, popup_pos, popup_pos + popup_size, draw->get_clr(clr->window.background_one));
        draw->line(draw_list, popup_pos + ImVec2(2, 2), popup_pos + ImVec2(popup_size.x - 2, 2), draw->get_clr(clr->accent));
        draw->line(draw_list, popup_pos + ImVec2(2, 3), popup_pos + ImVec2(popup_size.x - 2, 3), draw->get_clr(clr->accent, 0.4f));
        draw->rect(draw_list, popup_pos, popup_pos + popup_size, draw->get_clr(clr->window.stroke));
        auto text_size = var->font.tahoma->CalcTextSizeA(var->font.tahoma->FontSize, FLT_MAX, -1.f, label.data());
        draw->text_outline(draw_list, var->font.tahoma, var->font.tahoma->FontSize, cursor_pos + ImVec2(1, 0), draw->get_clr(clr->widgets.text), label.data());
        SetCursorPos({ elements->widgets.padding.x, elements->widgets.padding.y * 2 + text_size.y });
        return true;
    }

    if (*opened) *opened = false;
    gui->pop_style_var();
    gui->pop_style_color(2);
    EndGroup();
    PopID();

    return *opened;
}

void c_gui::end_extension() {
    gui->pop_style_var(2);
    EndPopup();
    gui->pop_style_var();
    gui->pop_style_color(2);
    EndGroup();
    PopID();
}