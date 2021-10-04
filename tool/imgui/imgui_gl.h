#pragma once

#include "imgui/imgui.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace ImGuiGl {
    IMGUI_API bool Init(HWND hWnd);
    IMGUI_API void Shutdown();
    IMGUI_API void NewFrame();

    IMGUI_API void InvalidateDeviceObjects();
    IMGUI_API bool CreateDeviceObjects();

    IMGUI_API void MouseDownCallback(int button);
    IMGUI_API void ScrollCallback(double xoffset, double yoffset);
    IMGUI_API void KeyCallback(int key, bool press);
    IMGUI_API void CharCallback(unsigned int c);
}
