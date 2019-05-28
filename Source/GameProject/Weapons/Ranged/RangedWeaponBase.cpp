// Copyright 2019 IvayloH. All rights reserved.

#include "RangedWeaponBase.h"
#include "Weapons/Ranged/ProjectileBase.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
ARangedWeaponBase::ARangedWeaponBase()
{
	WeaponSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Weapon"));
	WeaponSkeletalMeshComponent->SetCollisionProfileName("NoCollision");
	WeaponSkeletalMeshComponent->SetCanEverAffectNavigation(false);
	WeaponSkeletalMeshComponent->CanCharacterStepUpOn = ECB_No;
	
	RootComponent = WeaponSkeletalMeshComponent;

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(WeaponSkeletalMeshComponent);
}

void ARangedWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARangedWeaponBase::OnWeaponFire()
{
	if (ProjectileClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Tried to fire weapon without a projectile class set!"));
		return;
	}

	UWorld* const World = GetWorld();
	if (World)
	{
		const FRotator SpawnRotation = MuzzleLocation->GetComponentRotation();
		const FVector SpawnLocation = MuzzleLocation->GetComponentLocation();

		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		AProjectileBase* ProjectileReference = World->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParameters);
		ProjectileReference->SetupProjectileProperties(WeaponDamageDetails, GetInstigatorController(), this->GetAttachParentActor());
	}
}

void ARangedWeaponBase::SetWeaponDamageType(EDamageTypes NewDamageType)
{
	bool bDamageTypeChanged = WeaponDamageDetails.SetDamageEvent(NewDamageType);
	if (bDamageTypeChanged)
	{
		//TODO - add delegate to inform change to weapon damage type
	}
}


