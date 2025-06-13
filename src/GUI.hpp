#pragma once
#include "SDK.hpp"
#include "kiero.h"
#include <d3d11.h>
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "ui/UIWindow.hpp"
using namespace SDK;

// Taken from ImGui-DirectX-11-Kiero-Hook by rdbo on GitHub
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

namespace GUI {
    extern Present oPresent;
    extern HWND window;
    extern WNDPROC oWndProc;
    extern ID3D11Device* pDevice;
    extern ID3D11DeviceContext* pContext;
    extern ID3D11RenderTargetView* mainRenderTargetView;

    extern bool bInit;
    extern bool bMenuOpen;

    extern std::vector<UIWindow*> UIWindows;

    void InitImGui();
    LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
}