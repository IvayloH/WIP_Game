// Copyright 2019 IvayloH. All rights reserved.

#include "ItemBase.h"
#include "Components/BoxComponent.h"
#include "Characters/Player/PlayerCharacterBase.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	RootComponent = ObjectMesh;
	ObjectMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ObjectMesh->SetGenerateOverlapEvents(false);
	ObjectMesh->SetCanEverAffectNavigation(false);
	ObjectMesh->CanCharacterStepUpOn = ECB_No;

	CollisionBoundsBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoundsBox"));
	CollisionBoundsBox->SetupAttachment(RootComponent);
	CollisionBoundsBox->SetGenerateOverlapEvents(false);
	CollisionBoundsBox->SetCanEverAffectNavigation(false);
	CollisionBoundsBox->CanCharacterStepUpOn = ECB_No;
	CollisionBoundsBox->SetCollisionProfileName("Item");
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	//PickUpInRange->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnOverlapBegin);
}
/*
void AItemBase::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr && OtherActor != nullptr) && OtherComp != nullptr)
	{
		APlayerCharacterBase* PlayerCharacterRef = Cast<APlayerCharacterBase>(OtherActor);

		if (PlayerCharacterRef)
		{
			PlayerCharacterRef->OnItemPickedUp(PickUpItemType, AmountToChange);
			Destroy();
		}
	}
}
*/