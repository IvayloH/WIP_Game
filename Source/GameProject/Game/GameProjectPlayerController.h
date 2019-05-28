// Copyright 2019 IvayloH. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameProjectPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API AGameProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
};
