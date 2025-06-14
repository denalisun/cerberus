#pragma once
#include "Globals.hpp"
#include "SDK.hpp"

using namespace SDK;
using namespace Globals;

namespace Inventory
{
	void UpdateInventory();
	UFortWorldItem* FindItemByGuid(FGuid ItemGuid);
	UFortWorldItem* FindItemBySlot(int Slot);
	int GetOpenQuickBarSlot();
	UFortWorldItem* AddItemToInventory(UFortItemDefinition* ItemDef, int Slot, int Count = 1, EFortQuickBars QuickBar = EFortQuickBars::Primary);
	void EquipItemFromInventory(FGuid ItemGuid);
	void RemoveItemFromInventory(FGuid ItemGuid);
	void SpawnPickup(UFortItemDefinition* ItemDef, int Count, FVector Location, bool bTossedFromContainer = false, bool bToss = true);
	void DropItemFromInventory(FGuid ItemGuid, int Count);
	void PickupItem(AFortPickupAthena* Pickup);
	void InitializeInventory();
}