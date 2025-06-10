#include <iostream>
#include <Windows.h>
#include "FortSDK/SDK.hpp"
#include "Hooks.hpp"
#include "Utils.hpp"
using namespace SDK;

DWORD APIENTRY MainThread(HMODULE hModule) {
    AllocConsole();
    FILE* pFile;
    freopen_s(&pFile, "CONOUT$", "w", stdout);

    CLOG("Cerberus loaded!");

    uintptr_t baseAddr = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
    
    // Update globals, initialize hooks && create UE console
    Globals::Update();
    Hooks::InitializeHooks(baseAddr + Offsets::ProcessEvent);
    Globals::CreateConsole();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));

    return TRUE;
}