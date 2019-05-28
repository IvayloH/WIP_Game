// Copyright 2019 IvayloH. All rights reserved.

#include "ItemDropProbability.h"

TSubclassOf<AItemBase> UItemDropProbability::GetRandomItemToSpawn() const
{
	// Is there is a specific item we want to drop
	if (bMustSpawnSpecificItem && ItemToDrop != nullptr)
		return ItemToDrop;

	if (DropItems.Num() == 0)
		return nullptr;

	for (uint8 NumTryToDropItem = 0; NumTryToDropItem < NumTimesToTryItemDrop; ++NumTryToDropItem)
	{
		uint32 index = FMath::RandRange(0, DropItems.Num() - 1);
		//roll a dice to determine whether we should spawn it
		bool bShouldDropItem = FMath::FRandRange(0.f, 100.f) < DropItems[index].GetItemDropChance();
		if(bShouldDropItem)
			return DropItems[index].GetItemBlueprint();
	}
	//Exhausted all tries, nothing will spawn
	return nullptr;
}