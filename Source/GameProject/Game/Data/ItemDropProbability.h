// Copyright 2019 IvayloH. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Items/ItemBase.h"
#include "ItemDropProbability.generated.h"

USTRUCT()
struct FDropItem
{
	GENERATED_USTRUCT_BODY()
private:
	UPROPERTY(EditDefaultsOnly)
	FName ItemName;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AItemBase> ItemBlueprint;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float ItemDropChance;

public:
	FName GetItemName() const
	{
		return ItemName;
	}

	TSubclassOf<AItemBase> GetItemBlueprint() const
	{
		return ItemBlueprint;
	}

	float GetItemDropChance() const
	{
		return ItemDropChance;
	}
};
/**
 * Data Asset describing the items that a character can spawn on death.
 */
UCLASS()
class GAMEPROJECT_API UItemDropProbability : public UDataAsset
{
	GENERATED_BODY()

protected:
	/** Do we want to spawn a specific Item?*/
	UPROPERTY(EditDefaultsOnly)
	bool bMustSpawnSpecificItem;

	/** What Item do we want to spawn specifically?*/
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bMustSpawnSpecificItem"))
	TSubclassOf<AItemBase> ItemToDrop;

	/** How many times should we try to spawn a random item from the list?*/
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1.0", UIMin = "1.0"))
	uint8 NumTimesToTryItemDrop = 1;

	/** A list of all the items that can be dropped.*/
	UPROPERTY(EditDefaultsOnly)
	TArray<FDropItem> DropItems;

public:
	/** Retrieve the specified item to drop if it has been set or randomly retrieve an item from the drop list to spawn. Returns NULL if list is empty or no items should spawn.*/
	TSubclassOf<AItemBase> GetRandomItemToSpawn() const;
};
