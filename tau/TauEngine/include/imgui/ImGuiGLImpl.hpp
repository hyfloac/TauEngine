#pragma once

#include <imgui/imgui.h>

IMGUI_IMPL_API bool ImGui_ImplGL_Init() noexcept;

IMGUI_IMPL_API void ImGui_ImplGL_Render(ImDrawData* drawData) noexcept;

IMGUI_IMPL_API void ImGui_ImplGL_Destroy() noexcept;
