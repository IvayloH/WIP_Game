// Copyright 2019 IvayloH. All rights reserved.

#include "DamageOverTimeEffect.h"

UDamageOverTimeEffect::UDamageOverTimeEffect()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = DamageRate = 1.0f;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	CriticalStrikeMultiplier = 1.1f;
	CriticalStrikeChance = 5.0f;
}

void UDamageOverTimeEffect::BeginPlay()
{
	Super::BeginPlay();
	OwningActor = Cast<ACharacterBase>(GetOwner());
}

void UDamageOverTimeEffect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	bool bCriticalStrike = FMath::FRandRange(0.0f, 100.0f) < CriticalStrikeChance;
	float DamageToDeal = bCriticalStrike ? DamageOnTick * CriticalStrikeMultiplier : DamageOnTick;
	FDamageDetails DamageDetails(DamageToDeal, bCriticalStrike, 0.f, DamageEventType);
	OwningActor->TakeDamage(DamageDetails, nullptr, OwningActor);
}

void UDamageOverTimeEffect::BeginDealingDamage(struct FDamageEvent const& DamageEvent, const float DamageEffectDuration)
{
	DamageEventType = DamageEvent;
	PrimaryComponentTick.SetTickFunctionEnable(true);
	SetNegativeStatusEffect(DamageEventType);
	GetWorld()->GetTimerManager().SetTimer(DamageDealingTimerHandle, this, &UDamageOverTimeEffect::EndDealingDamage, DamageEffectDuration);
}

void UDamageOverTimeEffect::EndDealingDamage()
{
	GetWorld()->GetTimerManager().ClearTimer(DamageDealingTimerHandle);
	PrimaryComponentTick.SetTickFunctionEnable(false);
	SetNegativeStatusEffect(ENegativeStatusEffects::None);
}

float UDamageOverTimeEffect::GetDamageOnTick() const
{
	return DamageOnTick;
}

float UDamageOverTimeEffect::GetCriticalDamageChance() const
{
	return CriticalStrikeChance;
}

float UDamageOverTimeEffect::GetDamageRate() const
{
	return DamageRate;
}

float UDamageOverTimeEffect::GetCriticalDamageMultiplier() const
{
	return CriticalStrikeMultiplier;
}

void UDamageOverTimeEffect::SetDamageRate(const float NewDamageRate)
{
	DamageRate = DamageRate != NewDamageRate ? FMath::Clamp(NewDamageRate, .0f, 60.f) : DamageRate;
	PrimaryComponentTick.TickInterval = DamageRate;
}

void UDamageOverTimeEffect::SetDamageOnTick(float NewDamageOnTick)
{
	NewDamageOnTick = FMath::Clamp(NewDamageOnTick, .0f, 100.f);
	DamageOnTick = DamageOnTick != NewDamageOnTick ? NewDamageOnTick : DamageOnTick;
}

void UDamageOverTimeEffect::SetCriticalDamageChance(float NewCriticalDamageChance)
{
	NewCriticalDamageChance = FMath::Clamp(NewCriticalDamageChance, .0f, 100.f);
	CriticalStrikeChance = CriticalStrikeChance != NewCriticalDamageChance ? NewCriticalDamageChance : CriticalStrikeChance;
}

void UDamageOverTimeEffect::SetNegativeStatusEffect(ENegativeStatusEffects NewNegativeStatusEffect)
{
	if (NewNegativeStatusEffect != CurrentNegativeStatusEffect)
	{
		CurrentNegativeStatusEffect = NewNegativeStatusEffect;
		OnNegativeStatusEffectChanged.Broadcast(CurrentNegativeStatusEffect);
	}
}

ENegativeStatusEffects UDamageOverTimeEffect::GetCurrentNegativeStatusEffect() const
{
	return CurrentNegativeStatusEffect;
}

void UDamageOverTimeEffect::SetNegativeStatusEffect(struct FDamageEvent const& DamageEvent)
{
	if (DamageEvent.DamageTypeClass->IsChildOf(UPhysicalDamageType::StaticClass()))
		SetNegativeStatusEffect(ENegativeStatusEffects::Bleeding);
	else if (DamageEvent.DamageTypeClass->IsChildOf(UElectricDamageType::StaticClass()))
		SetNegativeStatusEffect(ENegativeStatusEffects::Electrified);
	else if (DamageEvent.DamageTypeClass->IsChildOf(UEnergyDamageType::StaticClass()))
		SetNegativeStatusEffect(ENegativeStatusEffects::Slowed);
	else if (DamageEvent.DamageTypeClass->IsChildOf(UFireDamageType::StaticClass()))
		SetNegativeStatusEffect(ENegativeStatusEffects::Burning);
	else if (DamageEvent.DamageTypeClass->IsChildOf(UIceDamageType::StaticClass()))
		SetNegativeStatusEffect(ENegativeStatusEffects::Freezing);
	else
		SetNegativeStatusEffect(ENegativeStatusEffects::Corroding);
}
