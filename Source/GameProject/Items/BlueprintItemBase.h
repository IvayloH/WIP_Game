// Copyright 2019 IvayloH. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "BlueprintItemBase.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API ABlueprintItemBase : public AItemBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly)
	UBlueprint* Item;

public:
	UBlueprint* GetItemBlueprint() const;
};
