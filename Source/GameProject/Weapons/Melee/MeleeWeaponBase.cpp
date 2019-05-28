// Copyright 2019 IvayloH. All rights reserved.

#include "MeleeWeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Enemy/EnemyCharacterBase.h"
#include "Characters/Player/PlayerCharacterBase.h"

AMeleeWeaponBase::AMeleeWeaponBase()
{
	WeaponSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SM_Weapon"));
	WeaponSkeletalMeshComponent->SetCollisionProfileName("NoCollision");
	WeaponSkeletalMeshComponent->SetCanEverAffectNavigation(false);
	WeaponSkeletalMeshComponent->CanCharacterStepUpOn = ECB_No;
	WeaponSkeletalMeshComponent->SetGenerateOverlapEvents(false);

	RootComponent = WeaponSkeletalMeshComponent;

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollisionCapsule"));
	CollisionCapsule->SetCollisionProfileName("Weapon");
	CollisionCapsule->SetCanEverAffectNavigation(false);
	CollisionCapsule->CanCharacterStepUpOn = ECB_No;
	CollisionCapsule->SetGenerateOverlapEvents(false);
	CollisionCapsule->SetupAttachment(RootComponent);
}

void AMeleeWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponBase::OnOverlapBegin);
}

float AMeleeWeaponBase::GetWeaponAttackDuration() const
{
	return WeaponAttackDuration;
}

USkeletalMeshComponent* AMeleeWeaponBase::GetSkeletalMeshComponent() const
{
	return WeaponSkeletalMeshComponent;
}

void AMeleeWeaponBase::EnableWeaponOverlapEvents()
{
	CollisionCapsule->SetGenerateOverlapEvents(true);
}

void AMeleeWeaponBase::DisableWeaponOverlapEvents()
{
	CollisionCapsule->SetGenerateOverlapEvents(false);
}

void AMeleeWeaponBase::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor==this->GetAttachParentActor())
		return;

	ACharacterBase* HitTarget = Cast<ACharacterBase>(OtherActor);
	if (HitTarget)
	{
		bool bCriticalStrike = FMath::FRandRange(0.f, 100.f) < GetWeaponCrticalStrikeChance();
		float DamageToDeal = bCriticalStrike ? (GetBaseWeaponDamage() * GetWeaponCriticalStrikeMultiplier()) : GetBaseWeaponDamage();
		FDamageDetails DamageDetails(DamageToDeal, bCriticalStrike, GetChanceToInflictDot(), GetWeaponDamageEventClass());

		HitTarget->TakeDamage(DamageDetails, GetAttachParentActor()->GetInstigatorController(), this);
	}
}