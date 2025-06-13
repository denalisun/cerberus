#pragma once
#pragma comment(lib, "libMinHook.x64.lib")
#include "SDK.hpp"
#include "Utils.hpp"
#include "Globals.hpp"
#include "Abilities.hpp"
#include "Inventory.hpp"
#include "Player.hpp"
#include "MinHook.h"
using namespace SDK;

// Including parameters
#include "SDK/FortniteGame_parameters.hpp"

namespace Hooks {
    extern void* (*ProcessEvent)(void*, void*, void*);
    void* ProcessEventHook(UObject* pObject, UFunction* pFunction, void* pParams);
    void InitializeHooks(uintptr_t processEventAddr);
}