#pragma once
#include "Globals.hpp"
#include "FortSDK/SDK.hpp"

using namespace SDK;
using namespace Globals;

namespace Player
{
	// this code is literally copied from Titan
	void SpawnPawn(FVector Location, FRotator Rotation)
	{
        FTransform trans;
        trans.Translation = Location;
        GPlayerController->Pawn = (AFortPlayerPawnAthena*)GGameplayStatics->FinishSpawningActor(GGameplayStatics->BeginDeferredActorSpawnFromClass(
            GWorld,
            APlayerPawn_Athena_C::StaticClass(),
            trans,
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn,
            GPlayerController
        ), trans);
		GPlayerPawn = (AFortPlayerPawnAthena*)GPlayerController->Pawn;

		GPlayerController->Possess(GPlayerPawn);

		GCheatManager->BugItGo(Location.X, Location.Y, Location.Z, Rotation.Pitch, Rotation.Yaw, Rotation.Roll);
	}

	// this code is literally copied from Titan
	void ShowSkin()
	{
		UFortRegisteredPlayerInfo* PlayerInfo = ((UFortGameInstance*)GEngine->GameInstance)->RegisteredPlayers[0];
		UFortHero* Hero = PlayerInfo->AthenaMenuHeroDef;

		AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)GPlayerController->PlayerState;
		PlayerState->HeroType = Hero->GetHeroTypeBP();
		PlayerState->OnRep_HeroType();

		for (int i = 0; i < Hero->CharacterParts.Num(); i++)
		{
			UCustomCharacterPart* Part = Hero->CharacterParts[i];

			if (!Part)
				continue;
			GPlayerPawn->ServerChoosePart(Part->CharacterPartType, Part);
		}

		PlayerState->CharacterBodyType = Hero->CharacterParts[1]->BodyTypesPermitted;
		GPlayerPawn->CharacterBodyType = Hero->CharacterParts[1]->BodyTypesPermitted;
		GPlayerPawn->CharacterGender = Hero->CharacterParts[1]->GenderPermitted;
		PlayerState->OnRep_CharacterParts();
	}

	void DeployFromBus()
	{
		auto Location = GPlayerController->GetFocalLocation();
		auto Rotation = GPlayerController->GetControlRotation();
		Rotation.Pitch = 0;
		Rotation.Roll = 0;

		Player::SpawnPawn(Location, Rotation);
		Player::ShowSkin();
	}
}