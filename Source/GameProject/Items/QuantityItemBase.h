// Copyright 2019 IvayloH. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "QuantityItemBase.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API AQuantityItemBase : public AItemBase
{
	GENERATED_BODY()

private:
	/** Do we want to use a percent value for the amount?*/
	UPROPERTY(EditDefaultsOnly)
	bool bUsePercentage;

	/** The amount of health/mana/ammo to give to the character.*/
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!bUsePercentage"))
	float ItemAmount;

	/** The percent amount health/mana/ammo to give to the character. E.g. A value of 25 health will restore 25% of max health to the character.*/
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bUsePercentage"))
	float ItemAmountPercent;

public:
	float GetItemAmount() const;
};
