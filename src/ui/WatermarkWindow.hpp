#pragma once
#include "SDK.hpp"
#include "kiero.h"
#include <d3d11.h>
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "UIWindow.hpp"
using namespace SDK;

class WatermarkWindow : public UIWindow {
public:
    const ImGuiWindowFlags WATERMARK_WINDOW_FLAGS = ImGuiWindowFlags_NoFocusOnAppearing |
                                                    ImGuiWindowFlags_AlwaysAutoResize   |
                                                    ImGuiWindowFlags_NoSavedSettings    |
                                                    ImGuiWindowFlags_NoDecoration       |
                                                    ImGuiWindowFlags_NoMove             |
                                                    ImGuiWindowFlags_NoNav;
    const int CORNER = 2; // Bottom Left
    const float DISTANCE_TO_CORNER = 15.0f;

    WatermarkWindow();
    void Draw() override;
};