#include "Inventory.hpp"
#include "Utils.hpp"

namespace Inventory
{
	void UpdateInventory()
	{
		GPlayerController->WorldInventory->HandleInventoryLocalUpdate();
		GPlayerController->HandleWorldInventoryLocalUpdate();

        GPlayerController->ClientForceUpdateQuickbar(EFortQuickBars::Primary);
        GPlayerController->ClientForceUpdateQuickbar(EFortQuickBars::Secondary);
	}

	UFortWorldItem* FindItemByGuid(FGuid ItemGuid)
	{
		for (int i = 0; i < GPlayerController->WorldInventory->Inventory.ItemInstances.Num(); i++)
		{
			FGuid IGuid = GPlayerController->WorldInventory->Inventory.ItemInstances[i]->GetItemGuid();
			if (IGuid.A == ItemGuid.A)
			{
				return GPlayerController->WorldInventory->Inventory.ItemInstances[i];
			}
		}
		
		return nullptr;
	}

	UFortWorldItem* FindItemBySlot(int Slot)
	{
		for (int i = 0; i < GPlayerController->ClientQuickBars->PrimaryQuickBar.Slots.Num(); i++)
		{
			std::cout << GPlayerController->ClientQuickBars->PrimaryQuickBar.Slots[i].Items.Num() << std::endl;
			if (GPlayerController->ClientQuickBars->PrimaryQuickBar.Slots[i].Items.Num() > 0)
			{
				FGuid ItemGuid = GPlayerController->ClientQuickBars->PrimaryQuickBar.Slots[i].Items[0];
				auto Item = FindItemByGuid(ItemGuid);
				if (Item)
					return Item;
			}
		}
		return nullptr;
	}

	UFortItem* CreateItem(UFortItemDefinition* ItemDef, int Count) {
		auto ItemInstance = ItemDef->CreateTemporaryItemInstanceBP(Count, 1);
		ItemInstance->SetOwningControllerForTemporaryItem(GPlayerController);
		return ItemInstance;
	}

	int GetOpenQuickBarSlot()
	{
		for (int i = 0; i < 6; i++)
		{
			auto Item = FindItemBySlot(i);
			std::cout << Item << std::endl;
			if (Item == nullptr)
				return i;
		}

		return -1;
	}

	UFortWorldItem* AddItemToInventory(UFortItemDefinition* ItemDef, int Slot, int Count, EFortQuickBars QuickBar)
	{
		if (!ItemDef)
			return nullptr;

		UFortWorldItem* Item = (UFortWorldItem*)CreateItem(ItemDef, Count);
		GPlayerController->WorldInventory->Inventory.ItemInstances.Add(Item);
		GPlayerController->WorldInventory->Inventory.ReplicatedEntries.Add(Item->ItemEntry);

		UpdateInventory();

		return Item;
	}

	void EquipItemFromInventory(FGuid ItemGuid)
	{
		for (int i = 0; i < GPlayerController->WorldInventory->Inventory.ItemInstances.Num(); i++)
		{
			FGuid IGuid = GPlayerController->WorldInventory->Inventory.ItemInstances[i]->GetItemGuid();
			if (IGuid.A == ItemGuid.A)
			{
				auto weapBP = GPlayerController->WorldInventory->Inventory.ItemInstances[i]->GetItemDefinitionBP();

				AFortWeapon* WeaponData = GPlayerPawn->EquipWeaponDefinition((UFortWeaponItemDefinition*)weapBP, IGuid);

				// stuff
				WeaponData->OnRep_ReplicatedWeaponData();
				WeaponData->ClientGivenTo(GPlayerPawn);
				GPlayerPawn->ClientInternalEquipWeapon(WeaponData);
			}
		}
	}

	void RemoveItemFromInventory(FGuid ItemGuid)
	{
		for (int i = 0; i < GPlayerController->WorldInventory->Inventory.ItemInstances.Num(); i++)
		{
			FGuid IGuid = GPlayerController->WorldInventory->Inventory.ItemInstances[i]->GetItemGuid();
			if (IGuid.A == ItemGuid.A)
			{
				GPlayerController->WorldInventory->Inventory.ItemInstances.Remove(i);
			}
		}

		for (int i = 0; i < GPlayerController->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
		{
			FGuid IGuid = GPlayerController->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid;
			if (IGuid.A == ItemGuid.A)
			{
				GPlayerController->WorldInventory->Inventory.ReplicatedEntries.Remove(i);
			}
		}

		//GPlayerController->ClientQuickBars->ServerRemoveItemInternal(ItemGuid, true, true);
	}

	void SpawnPickup(UFortItemDefinition* ItemDef, int Count, FVector Location, bool bTossedFromContainer, bool bToss)
	{
		auto SpawnTransform = FTransform{};
		SpawnTransform.Translation = Location;
		SpawnTransform.Scale3D = { 1,1,1 };

		auto FortPickupAthena = reinterpret_cast<AFortPickupAthena*>(GGameplayStatics->FinishSpawningActor(GGameplayStatics->BeginDeferredActorSpawnFromClass(GWorld, AFortPickupAthena::StaticClass(), SpawnTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, nullptr), SpawnTransform));

		FortPickupAthena->bTossedFromContainer = bTossedFromContainer;
		if (bTossedFromContainer)
			FortPickupAthena->OnRep_TossedFromContainer();

		FortPickupAthena->TossPickup(Location, nullptr, 1, bToss, EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::Unset);

		FortPickupAthena->PrimaryPickupItemEntry.Count = Count;
		FortPickupAthena->PrimaryPickupItemEntry.ItemDefinition = ItemDef;

		FortPickupAthena->OnRep_PrimaryPickupItemEntry();
	}

	void DropItemFromInventory(FGuid ItemGuid, int Count)
	{
		for (int i = 0; i < GPlayerController->WorldInventory->Inventory.ItemInstances.Num(); i++)
		{
			FGuid IGuid = GPlayerController->WorldInventory->Inventory.ItemInstances[i]->GetItemGuid();
			if (IGuid.A == ItemGuid.A)
			{
				auto weapBP = GPlayerController->WorldInventory->Inventory.ItemInstances[i]->GetItemDefinitionBP();

				SpawnPickup(weapBP, Count, GPlayerPawn->K2_GetActorLocation());

				RemoveItemFromInventory(ItemGuid);

				UpdateInventory();
			}
		}
	}

	void PickupItem(AFortPickupAthena* Pickup)
	{
		int QuickBarSlot = GetOpenQuickBarSlot();
		if (QuickBarSlot != -1)
			Inventory::AddItemToInventory(Pickup->PrimaryPickupItemEntry.ItemDefinition, QuickBarSlot, Pickup->PrimaryPickupItemEntry.Count, EFortQuickBars::Primary);
		else
		{
			auto CurrentFocusedSlot = GPlayerController->ClientQuickBars->PrimaryQuickBar.CurrentFocusedSlot;	

			if (CurrentFocusedSlot != 0)
			{
				Inventory::DropItemFromInventory(FindItemBySlot(CurrentFocusedSlot)->GetItemGuid(), 1);

				Inventory::AddItemToInventory(Pickup->PrimaryPickupItemEntry.ItemDefinition, CurrentFocusedSlot, Pickup->PrimaryPickupItemEntry.Count, EFortQuickBars::Primary);
			}
		}

		Pickup->K2_DestroyActor();
	}

	void InitializeInventory()
	{
		GPlayerController->ClientQuickBars->SetOwner(GPlayerController);
		UpdateInventory();
	}
}