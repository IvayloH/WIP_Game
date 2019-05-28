// Copyright 2019 IvayloH. All rights reserved.

#include "Characters/CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/Melee/MeleeWeaponBase.h"
#include "Weapons/Ranged/RangedWeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapons/DamageTypes/DamageOverTimeEffect.h"
#include "GameFramework/DamageType.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.SetTickFunctionEnable(false);

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	PlayerMovementPassiveAbility = EPlayerMovementPassiveAbilities::SingleJump;

	DashParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DashParticleSystem"));
	DashParticleSystem->SetupAttachment(RootComponent);
	DashParticleSystem->bAutoActivate = false;

	CharacterMeshReference = GetMesh();

	//Movement Default Values
	GroundDashVelocity = 3500.f;
	AirDashVelocity = 1500.f;

	//Health Default Values
	CurrentHealth = MaxHealth = 100.0f;

	DamageOverTimeComponent = CreateDefaultSubobject<UDamageOverTimeEffect>(TEXT("DamageOverTimeComponent"));
	DamageOverTimeComponent->SetDamageRate(1.0f);
	DamageOverTimeComponent->SetDamageOnTick(5.0f);

	//initialize the maps
	for (int32 index = 0; index < DamageTypesEnumSize; ++index)
	{
		DamageTypeResistances.Add((EDamageTypes)index, 5.0f);
		DamageTypeAbsorptionChance.Add((EDamageTypes)index, 5.0f);
		DamageOverTimeDurationByDamageType.Add((EDamageTypes)index, 5.0f);
	}

	//Layer tracing default values
	LayerTraceStartOffsetZ = 200.0f;
	LayerTraceLength = 100.f;
	bCanCharacterClimbUpLedges = true;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	//TODO: this should most likely check for a save game before attemtping to spawn a weapon, or check for a valid current weapon?
	if (bSpawnCharacterWithMeleeWeapon && MeleeWeaponBlueprints.Num() != 0)
	{
		MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeaponBase>(MeleeWeaponBlueprints[0]);
		MeleeWeapon->AttachToComponent(CharacterMeshReference, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), WeaponSocketName);
		CurrentMeleeWeapon = MeleeWeapon;
	}
	else if (bSpawnCharacterWithRangedWeapon && RangedWeaponBlueprints.Num() != 0)
	{
		RangedWeapon = GetWorld()->SpawnActor<ARangedWeaponBase>(RangedWeaponBlueprints[0]);
		RangedWeapon->AttachToComponent(CharacterMeshReference, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), WeaponSocketName);
		CurrentRangedWeapon = RangedWeapon;
	}
}

void ACharacterBase::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Is the character facing away from the ledge we want to climb up on?
	if (!LedgeClimbingForwardDirection.Equals(this->GetActorForwardVector(), 0.1f))
	{
		SetActorTickEnabled(false);
		return;
	}

	FVector TraceStartLocation = CharacterMeshReference->GetComponentLocation() + FVector(0.f, 0.f, LayerTraceStartOffsetZ);
	FVector TraceEndLocation = TraceStartLocation + FVector(0.0f, (LedgeClimbingForwardDirection.Y * LayerTraceLength), 0.f);
	FHitResult TraceHitResult(ForceInit);
	FCollisionQueryParams JumpTrace(TEXT("JumpTrace"), false, this);

	bool bTraceHit = GetWorld()->LineTraceSingleByChannel(
		TraceHitResult,
		TraceStartLocation,
		TraceEndLocation,
		LedgeClimbingTraceChannel,
		JumpTrace);

	if (bTraceHit)
	{
		bLastTraceHit = true;
		LastLayerTraceHitResult = TraceHitResult;
	}
	else if (bLastTraceHit && (GetCharacterMovement()->Velocity.Z > 0.0f) && (LastLayerTraceHitResult.Distance > 1.0f))
	{
		ClimbUpLedge(LastLayerTraceHitResult);
		bLastTraceHit = false;
		SetActorTickEnabled(false);
	}
	else
		bLastTraceHit = false;
}

void ACharacterBase::Jump()
{
	bPressedJump = true;
	JumpKeyHoldTime = 0.1f;

	if (bCanCharacterClimbUpLedges)
	{
		LastLayerTraceHitResult.Reset();
		LedgeClimbingForwardDirection = GetActorForwardVector();
		SetActorTickEnabled(true);
	}
}

void ACharacterBase::StopJumping()
{
	bPressedJump = false;
	ResetJumpState();
}

void ACharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	SetActorTickEnabled(false);
	bLastTraceHit = false;
}

void ACharacterBase::ClimbUpLedge(FHitResult HitResult)
{
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.CallbackTarget = this;
	FVector MoveToTargetLocation = HitResult.Location + FVector(0.f, GetActorForwardVector().Y * 15, 150.f);

	UKismetSystemLibrary::MoveComponentTo(
		GetCapsuleComponent(),
		MoveToTargetLocation,
		GetActorRotation(),
		false, false, 0.3f, false, EMoveComponentAction::Move, LatentActionInfo);

	GetCharacterMovement()->StopMovementImmediately();
}

void ACharacterBase::BeginDash()
{
	if (GetWorldTimerManager().IsTimerActive(DashHandle)
		|| (abs(GetVelocity().Y) < 0.1f)
		|| (JumpCurrentCount > 0 && !bCanDashMidJump))
		return;

	//stop the attack animation before we dash
	if (bIsMeleeAttacking)
		EndMeleeAttack();

	bCanCharacterTakeDamage = false;

	DashParticleSystem->Activate();
	if (IsPlayerControlled())
		DisableInput(GetWorld()->GetFirstPlayerController());

	CharacterMeshReference->SetHiddenInGame(true);

	GetWorldTimerManager().SetTimer(DashHandle, this, &ACharacterBase::EndDash, 0.3f);

	//if we're in the air, use the AirDashVelocity value, otherwise use GroundDashVelocity
	float DashVelocityMultiplier = JumpCurrentCount > 0 ? AirDashVelocity : GroundDashVelocity;
	GetCharacterMovement()->Velocity.Set(0.f, DashVelocityMultiplier*GetActorForwardVector().Y, 0.f);
}

void ACharacterBase::EndDash()
{
	CharacterMeshReference->SetHiddenInGame(false);
	if(IsPlayerControlled())
		EnableInput(GetWorld()->GetFirstPlayerController());

	DashParticleSystem->Deactivate();

	bCanCharacterTakeDamage = true;
}

void ACharacterBase::OnItemPickedUp(EItemTypes PickedupItemType, float Amount)
{
	//TODO
	switch (PickedupItemType)
	{
	case EItemTypes::Health:
		break;
		
	default:
		break;
	}
}

void ACharacterBase::OnItemPickedUp(EItemTypes PickedupItemType, UBlueprint Blueprint)
{
	//TODO
}

void ACharacterBase::BeginMeleeAttack()
{
	//if we're already attacking or in the middle of a dash, don't do anything
	if (bIsMeleeAttacking
		|| GetWorldTimerManager().IsTimerActive(DashHandle)
		|| !CurrentMeleeWeapon)
		return;

	bIsMeleeAttacking = true;
	MeleeWeapon->EnableWeaponOverlapEvents();
	OnMeleeAttackStateChanged.Broadcast(bIsMeleeAttacking);

	GetWorldTimerManager().SetTimer(MeleeAttackTimerHandle, this, &ACharacterBase::EndMeleeAttack, MeleeWeapon->GetWeaponAttackDuration());
}

void ACharacterBase::EndMeleeAttack()
{
	//If we've started an attack and then interrupted it while the timer is still active, stop the timer
	GetWorldTimerManager().ClearTimer(MeleeAttackTimerHandle);

	MeleeWeapon->DisableWeaponOverlapEvents();
	bIsMeleeAttacking = false;
	OnMeleeAttackStateChanged.Broadcast(bIsMeleeAttacking);
}

float ACharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACharacterBase::TakeDamage(struct FDamageDetails const& DamageDetails, class AController* EventInstigator, AActor* DamageCauser)
{
	FDamageEvent DamageEvent = DamageDetails.GetDamageEvent();
	float ActualDamage = TakeDamage(DamageDetails.GetDamageAmount(), DamageEvent, EventInstigator, DamageCauser);

	bool bDamageFullyAbsorbed = false;

	EDamageTypes DamageTypeDealt;
	if (DamageEvent.DamageTypeClass->IsChildOf(UPhysicalDamageType::StaticClass()))
		DamageTypeDealt = EDamageTypes::Physical;
	else if (DamageEvent.DamageTypeClass->IsChildOf(UElectricDamageType::StaticClass()))
		DamageTypeDealt = EDamageTypes::Electric;
	else if (DamageEvent.DamageTypeClass->IsChildOf(UEnergyDamageType::StaticClass()))
		DamageTypeDealt = EDamageTypes::Energy;
	else if (DamageEvent.DamageTypeClass->IsChildOf(UFireDamageType::StaticClass()))
		DamageTypeDealt = EDamageTypes::Fire;
	else if (DamageEvent.DamageTypeClass->IsChildOf(UIceDamageType::StaticClass()))
		DamageTypeDealt = EDamageTypes::Ice;
	else
		DamageTypeDealt = EDamageTypes::Acid;

	// Try to absorb the damage from this attack
	if (FMath::FRandRange(0.f, 100.f) < GetDamageTypeAbsorptionChance(DamageTypeDealt))
	{
		//Absorb the damage
		ActualDamage = 0.f;
		bDamageFullyAbsorbed = true;
	}
	else
	{	//Adjust damage to account for any damage resistance
		ActualDamage *= (1.f - GetDamageTypeResistance(DamageTypeDealt));
		CurrentHealth = FMath::Max(0.0f, CurrentHealth - ActualDamage);

		//ensure damage was caused by something/someone else
		if (DamageCauser && (DamageCauser != this) && GetCurrentNegativeStatusEffect() == ENegativeStatusEffects::None)
		{
			if (FMath::FRandRange(0.f, 100.f) < DamageDetails.GetChanceToInflictDot()) // roll dice to inflict a DoT effect
			{
				DamageOverTimeComponent->BeginDealingDamage(DamageEvent, GetDamageOverTimeDurationByDamageType(DamageTypeDealt));
			}
		}

		if (CurrentHealth <= 0.f)
		{
			CharacterDeath();
		}
	}

	OnPostDamageTaken.Broadcast(DamageDetails.GetDamageAmount(), ActualDamage, DamageDetails.IsCriticalStrike(), bDamageFullyAbsorbed, DamageTypeDealt, CurrentHealth);
}

float ACharacterBase::GetMaxHealth() const
{
	return MaxHealth;
}

float ACharacterBase::GetCurrentHealth() const
{
	return CurrentHealth;
}

void ACharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void ACharacterBase::Heal(float AmountToHeal)
{
	CurrentHealth = FMath::Min(CurrentHealth + AmountToHeal, MaxHealth);
}

void ACharacterBase::UpdateDamageTypeResistance(EDamageTypes DamageTypeResistanceToModify, const float NewResistanceValue)
{
	DamageTypeResistances.Add(DamageTypeResistanceToModify, FMath::Clamp(NewResistanceValue, 0.0f, 100.0f));
}

float ACharacterBase::GetDamageTypeResistance(EDamageTypes DamageTypeToLookUp)
{
	float* DmgTypeResistance = DamageTypeResistances.Find(DamageTypeToLookUp);
	return DmgTypeResistance != nullptr ? (*DmgTypeResistance / 100.f) : 0.0f;
}

float ACharacterBase::GetDamageTypeAbsorptionChance(EDamageTypes DamageTypeToLookUp)
{
	float* DmgTypeAbsorption = DamageTypeAbsorptionChance.Find(DamageTypeToLookUp);
	return DmgTypeAbsorption != nullptr ? (*DmgTypeAbsorption / 100.f) : 0.0f;
}

float ACharacterBase::GetDamageOverTimeDurationByDamageType(EDamageTypes DamageTypeToLookUp)
{
	float* DamageOverTimeDuration = DamageTypeAbsorptionChance.Find(DamageTypeToLookUp);
	return DamageOverTimeDuration != nullptr ? *DamageOverTimeDuration : 0.0f;
}

ENegativeStatusEffects ACharacterBase::GetCurrentNegativeStatusEffect() const
{
	return DamageOverTimeComponent->GetCurrentNegativeStatusEffect();
}

void ACharacterBase::CharacterDeath()
{
	GetMesh()->SetCollisionProfileName("NoCollision");
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	DamageOverTimeComponent->EndDealingDamage();
}