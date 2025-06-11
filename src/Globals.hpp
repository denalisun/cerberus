#pragma once
#include "FortSDK/SDK.hpp"
using namespace SDK;

namespace Globals
{
	// Game variables
	UFortEngine* GEngine;
	UWorld* GWorld;
	AFortPlayerControllerAthena* GPlayerController;
	AFortPlayerPawnAthena* GPlayerPawn;
	UGameplayStatics* GGameplayStatics;
	UCheatManager* GCheatManager;
	UFortInventoryContext* GInventoryContext;

	USoundCue* ChestOpenSound;

	bool InGame = false;
	bool LoadingScreenDropped = false;

	void CreateConsole() {
		GEngine->GameViewport->ViewportConsole = reinterpret_cast<UConsole*>(GGameplayStatics->SpawnObject(UConsole::StaticClass(), GEngine->GameViewport));
	}

	UCheatManager* CreateCheatManager() {
		if (GPlayerController->CheatManager == nullptr)
			GPlayerController->CheatManager = reinterpret_cast<UCheatManager*>(GGameplayStatics->SpawnObject(UCheatManager::StaticClass(), GPlayerController));
		return GPlayerController->CheatManager;
	}

	AActor* SpawnActor(AActor* ActorClass, FVector Location, FRotator Rotation) {
        FTransform trans;
		trans.Translation = Location;
		auto Actr = GGameplayStatics->FinishSpawningActor(
			GGameplayStatics->BeginDeferredActorSpawnFromClass(
				GWorld,
				ActorClass->StaticClass(),
				trans,
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn,
				nullptr
			), trans
		);
		Actr->K2_TeleportTo(Location, Rotation);

        return Actr;
	}

	AActor* SpawnActorFromClassName(std::string ActorClassName, FVector Location, FRotator Rotation) {
		AActor* Actr = nullptr;
		
		auto ActorClass = UObject::FindObject<AActor>(ActorClassName);
		if (ActorClass != nullptr)
			Actr = SpawnActor(ActorClass, Location, Rotation);

		return Actr;
	}

	void Update()
	{
		GEngine = UObject::FindObject<UFortEngine>("FortEngine Transient.FortEngine_2147482594");
		GWorld = GEngine->GameViewport->World;
		GPlayerController = (AFortPlayerControllerAthena*)GEngine->GameInstance->LocalPlayers[0]->PlayerController;
		GPlayerPawn = (AFortPlayerPawnAthena*)GPlayerController->Pawn;
		GGameplayStatics = UObject::FindObject<UGameplayStatics>("GameplayStatics Engine.Default__GameplayStatics");
		GInventoryContext = (UFortInventoryContext*)GEngine;

		GCheatManager = GPlayerController->CheatManager = Globals::CreateCheatManager();

		ChestOpenSound = UObject::FindObject<USoundCue>("SoundCue Tiered_Chest_Open_T01_Cue.Tiered_Chest_Open_T01_Cue");
	}
}