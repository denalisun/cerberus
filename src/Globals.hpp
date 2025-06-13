#pragma once
#include "SDK.hpp"
using namespace SDK;

namespace Globals
{
	// Game variables
	extern bool InGame;
	extern bool LoadingScreenDropped;

	extern UFortEngine* GEngine;
	extern UWorld* GWorld;
	extern AFortPlayerControllerAthena* GPlayerController;
	extern AFortPlayerPawnAthena* GPlayerPawn;
	extern UGameplayStatics* GGameplayStatics;
	extern UCheatManager* GCheatManager;
	extern UFortInventoryContext* GInventoryContext;
	extern USoundCue* ChestOpenSound;

	UCheatManager* CreateCheatManager();
	AActor* SpawnActor(AActor* ActorClass, FVector Location, FRotator Rotation);
	AActor* SpawnActorFromClassName(std::string ActorClassName, FVector Location, FRotator Rotation);
	void Update();
	void CreateConsole();
}