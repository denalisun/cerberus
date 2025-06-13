#pragma once
#include "SDK.hpp"
#include "kiero.h"
#include <d3d11.h>
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
using namespace SDK;

class UIWindow {
public:
    bool m_bIsOpen;

    UIWindow();
    virtual void Draw();
    virtual void Tick();
};