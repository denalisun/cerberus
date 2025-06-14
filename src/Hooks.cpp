#include "Hooks.hpp"

namespace Hooks {
    void* (*ProcessEvent)(void*, void*, void*);

    void* ProcessEventHook(UObject* pObject, UFunction* pFunction, void* pParams)
    {
        if (pObject != nullptr && pFunction != nullptr)
        {
            std::string funcName = pFunction->GetName();

            if (funcName.find("ReadyToStartMatch") != std::string::npos && !Globals::InGame)
            {
                Globals::InGame = true;
                Globals::Update();

                CLOG("Updated Globals!");

                AFortGameModeAthena* GameMode = (AFortGameModeAthena*)pObject;
                AFortGameStateAthena* GameState = (AFortGameStateAthena*)GameMode->GameState;

                CLOG("Grabbed GameMode and GameState!");

                //UFortPlaylistAthena* Playlist = (UFortPlaylistAthena*)Globals::FindObject("Playlist_DefaultSolo.Playlist_DefaultSolo");
                //GameState->CurrentPlaylistInfo.BasePlaylist = Playlist;
                //GameState->CurrentPlaylistInfo.OverridePlaylist = Playlist;
                //GameState->OnRep_CurrentPlaylistInfo();
                
                GameState->AircraftStartTime = 100.f;
                GameState->WarmupCountdownEndTime = 100.f;
                GameState->GamePhase = EAthenaGamePhase::Warmup;
                GameState->OnRep_GamePhase(EAthenaGamePhase::None);

                // Spawning the pawn
                Player::SpawnPawn({1,1,10000}, { 0,0,0 });
                
                //GameMode->StartMatch();
                Globals::GPlayerController->ServerSetClientHasFinishedLoading(true);
                Globals::GPlayerController->bHasClientFinishedLoading = true;
                Globals::GPlayerController->bHasServerFinishedLoading = true;
                Globals::GPlayerController->OnRep_bHasServerFinishedLoading();
                Globals::GPlayerController->ServerReadyToStartMatch();
                
                // Show skin
                Globals::GPlayerPawn->OnRep_CustomizationLoadout();
                Player::ShowSkin();
                Globals::GCheatManager->DestroyAll(AFortHLODSMActor::StaticClass());
            }

            if (funcName.find("ServerLoadingScreenDropped") != std::string::npos && Globals::LoadingScreenDropped == false)
            {
                Globals::LoadingScreenDropped = true;

                // Initializing inventory
                Inventory::InitializeInventory();

                auto PickaxeDef = Globals::GPlayerController->CustomizationLoadout.Pickaxe->WeaponDefinition;
                if (PickaxeDef != nullptr)
                    Inventory::AddItemToInventory(PickaxeDef, 0);
                CLOG("PickaxeDef: " + std::string(reinterpret_cast<const char*>(PickaxeDef)));

                // Looping through DefaultPlayer and applying all abilities.
                Abilities::GiveAbility(UObject::FindObject<UGameplayAbility>("Class FortniteGame.FortGameplayAbility_Jump"));
                Abilities::GiveAbility(UObject::FindObject<UGameplayAbility>("Class FortniteGame.FortGameplayAbility_Sprint"));
                Abilities::GiveAbility(UObject::FindObject<UGameplayAbility>("BlueprintGeneratedClass GA_DefaultPlayer_InteractSearch.GA_DefaultPlayer_InteractSearch_C"));
                Abilities::GiveAbility(UObject::FindObject<UGameplayAbility>("BlueprintGeneratedClass GA_DefaultPlayer_InteractUse.GA_DefaultPlayer_InteractUse_C"));

                // //TODO: Add building WIDs
                // Inventory::AddItemToInventory(UObject::FindObject<UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Wall.BuildingItemData_Wall"), 0);
                // Inventory::AddItemToInventory(UObject::FindObject<UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Floor.BuildingItemData_Floor"), 1);
                // Inventory::AddItemToInventory(UObject::FindObject<UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Stair_W.BuildingItemData_Stair_W"), 2);
                // Inventory::AddItemToInventory(UObject::FindObject<UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_RoofS.BuildingItemData_RoofS"), 3);

                // Add resources
                // Inventory::AddItemToInventory(UObject::FindObject<UFortResourceItemDefinition>("FortResourceItemDefinition WoodItemData.WoodItemData"), 0, 999);
                // Inventory::AddItemToInventory(UObject::FindObject<UFortResourceItemDefinition>("FortResourceItemDefinition StoneItemData.StoneItemData"), 0, 999);
                // Inventory::AddItemToInventory(UObject::FindObject<UFortResourceItemDefinition>("FortResourceItemDefinition MetalItemData.MetalItemData"), 0, 999);

                // // OK lets try minimap hopefully it works
                UTexture2D* MiniMapTexture = UObject::FindObject<UTexture2D>("Texture2D MiniMapAthena.MiniMapAthena");
                AFortGameStateAthena* GameState = (AFortGameStateAthena*)Globals::GEngine->GameViewport->World->GameState;
                GameState->MinimapBackgroundBrush.ResourceObject = MiniMapTexture;
            }
            
            if (funcName.find("ServerExecuteInventoryItem") != std::string::npos)
            {
                FGuid* lol = reinterpret_cast<FGuid*>(pParams);
                Inventory::EquipItemFromInventory(*lol);
            }

            if (funcName.find("ServerAttemptInventoryDrop") != std::string::npos)
            {
                auto Params = (Params::FortPlayerController_ServerAttemptInventoryDrop*)pParams;
                
                Inventory::DropItemFromInventory(Params->ItemGuid, Params->Count);
            }

            if (funcName.find("ServerHandlePickup") != std::string::npos && funcName.find("ServerHandlePickupWithSwap") == std::string::npos)
            {
                auto Params = (Params::FortPlayerPawn_ServerHandlePickup*)pParams;

                Inventory::PickupItem((AFortPickupAthena*)Params->Pickup);
            }
            
            if (funcName.find("ServerAttemptAircraftJump") != std::string::npos)
            {
                Player::DeployFromBus();
            }
        }

        return ProcessEvent(pObject, pFunction, pParams);
    }

    void InitializeHooks(uintptr_t processEventAddr)
    {
        if (MH_Initialize() != MH_OK) {
            CLOG("Error: MinHook NOT OK");
            return;
        }
        CLOG("MinHook OK");

        MH_CreateHook((LPVOID)processEventAddr, ProcessEventHook, reinterpret_cast<LPVOID*>(&ProcessEvent));
        MH_EnableHook((LPVOID)processEventAddr);
    }
}