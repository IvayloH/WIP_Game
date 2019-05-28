// Copyright 2019 IvayloH. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "EnemyCharacterBase.generated.h"

UCLASS()
class GAMEPROJECT_API AEnemyCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacterBase();

protected:
	/** Death animation to be played when this actor dies (health points reach 0).*/
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimSequence* DeathAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Item Drop")
	class UItemDropProbability* ItemDropProbability;

	virtual void CharacterDeath() override;
};
