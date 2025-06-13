#include <iostream>
#include <Windows.h>
#include "Hooks.hpp"
#include "Utils.hpp"
#include "ui/AllWindowIncludes.hpp"
#include "GUI.hpp"
using namespace SDK;

DWORD APIENTRY MainThread(HMODULE hModule) {
    AllocConsole();
    FILE* pFile;
    freopen_s(&pFile, "CONOUT$", "w", stdout);

    CLOG("Cerberus loaded!");

    uintptr_t baseAddr = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
    CLOG("Base address: " + std::to_string(baseAddr)); // I hope this doesn't crash
    
    // Update globals, initialize hooks && create UE console
    Globals::Update();
    Hooks::InitializeHooks(baseAddr + Offsets::ProcessEvent);
    Globals::CreateConsole();

    // Initialize ImGui
    bool init_hook = false;
    do {
        if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
            kiero::bind(8, (void**)& GUI::oPresent, GUI::hkPresent);
            init_hook = true;
        }
    } while (!init_hook);

    // Construct All Windows
    WatermarkWindow* watermarkWindow = new WatermarkWindow();
    CerberusMenu* cerberusMenu = new CerberusMenu();

    // Add all windows to the UIWindows vector in GUI
    GUI::UIWindows.push_back(watermarkWindow);
    GUI::UIWindows.push_back(cerberusMenu);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
            break;
        case DLL_PROCESS_DETACH:
            kiero::shutdown();
            break;
    }

    return TRUE;
}