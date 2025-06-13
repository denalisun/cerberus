#include "GUI.hpp"

namespace GUI {
    Present oPresent;
    HWND window = NULL;
    WNDPROC oWndProc;
    ID3D11Device* pDevice = NULL;
    ID3D11DeviceContext* pContext = NULL;
    ID3D11RenderTargetView* mainRenderTargetView;

    bool bInit = false;
    bool bMenuOpen = false;

    std::vector<UIWindow*> UIWindows = std::vector<UIWindow*>();

    void InitImGui() {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
        ImGui_ImplWin32_Init(window);
        ImGui_ImplDX11_Init(pDevice, pContext);
    }

    LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
            return true;

        return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
    }

    HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
        if (!bInit) {
            if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice))) {
                pDevice->GetImmediateContext(&pContext);
                DXGI_SWAP_CHAIN_DESC sd;
                pSwapChain->GetDesc(&sd);
                window = sd.OutputWindow;
                ID3D11Texture2D* pBackBuffer;
                pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
                pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
                pBackBuffer->Release();
                oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
                InitImGui();
                bInit = true;
            }
            else
                return oPresent(pSwapChain, SyncInterval, Flags);
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        for(int i = 0; i < UIWindows.size(); i++) {
            UIWindows.at(i)->Tick();
            if (UIWindows.at(i)->m_bIsOpen)
                UIWindows.at(i)->Draw();
        }

        ImGui::Render();

        pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        return oPresent(pSwapChain, SyncInterval, Flags);
    }
}