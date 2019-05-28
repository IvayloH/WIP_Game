// Copyright 2019 IvayloH. All rights reserved.

#include "EnemyCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Game/Data/ItemDropProbability.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyCharacterBase::AEnemyCharacterBase()
{
	
}

void AEnemyCharacterBase::CharacterDeath()
{
	Super::CharacterDeath();

	if (ItemDropProbability != nullptr)
	{
		TSubclassOf<class AItemBase> DropItem = ItemDropProbability->GetRandomItemToSpawn();
		if (DropItem)
		{
			FActorSpawnParameters ItemSpawnParams;
			ItemSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			GetWorld()->SpawnActor<AItemBase>(DropItem, GetMesh()->GetComponentTransform(), ItemSpawnParams);
		}
	}
}