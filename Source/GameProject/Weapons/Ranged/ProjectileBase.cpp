// Copyright 2019 IvayloH. All rights reserved.

#include "ProjectileBase.h"
#include "Characters/CharacterBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMeshComponent->SetCollisionProfileName("NoCollision");
	ProjectileMeshComponent->SetGenerateOverlapEvents(false);
	ProjectileMeshComponent->SetCanEverAffectNavigation(false);
	ProjectileMeshComponent->CanCharacterStepUpOn = ECB_No;

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphereComponent"));
	CollisionSphereComponent->SetSphereRadius(15.0f);
	CollisionSphereComponent->SetCollisionProfileName("Projectile");
	CollisionSphereComponent->SetCanEverAffectNavigation(false);
	CollisionSphereComponent->CanCharacterStepUpOn = ECB_No;
	CollisionSphereComponent->SetupAttachment(RootComponent);

	RootComponent = CollisionSphereComponent;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionSphereComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphereComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//TODO Implement a piercing option so the projectile can go through multiple enemies (should probably depend on enemy armor etc.)
	if (OtherActor != nullptr && OtherActor != this)
	{
		ACharacterBase* CharacterTargetHit = Cast<ACharacterBase>(OtherActor);
		if (CharacterTargetHit)
		{
			bool bIsCriticalStrike = FMath::FRandRange(0.f, 100.f) < WeaponDamageDetails.GetCriticalStrikeChance();
			float DamageAmount = bIsCriticalStrike ? (WeaponDamageDetails.GetBaseDamage() * WeaponDamageDetails.GetCriticalStrikeDamageMultiplier()) : WeaponDamageDetails.GetBaseDamage();
			DamageDetails = FDamageDetails(DamageAmount, bIsCriticalStrike, WeaponDamageDetails.GetChanceToInflictDot(), WeaponDamageDetails.GetEquivalentDamageEvent());
			
			CharacterTargetHit->TakeDamage(DamageDetails, InstantiatorController, Instantiator);
		}
	}
}

void AProjectileBase::SetupProjectileProperties(FWeaponDamageDetails const& WeaponDamageDetails, AController* const InstantiatorController, AActor* const Instantiator)
{
	this->WeaponDamageDetails = WeaponDamageDetails;
	this->InstantiatorController = InstantiatorController;
	this->Instantiator = Instantiator;
}
