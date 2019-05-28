// Copyright 2019 IvayloH. All rights reserved.

#include "GameProjectPlayerController.h"

void AGameProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameAndUI());
}