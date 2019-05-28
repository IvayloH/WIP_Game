// Copyright 2019 IvayloH. All rights reserved.

#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentDamageEventClass = WeaponDamageDetails.GetEquivalentDamageEvent();
}

float AWeaponBase::GetBaseWeaponDamage() const
{
	return WeaponDamageDetails.GetBaseDamage();
}

float AWeaponBase::GetChanceToInflictDot() const
{
	return WeaponDamageDetails.GetChanceToInflictDot();
}

float AWeaponBase::GetWeaponCrticalStrikeChance() const
{
	return WeaponDamageDetails.GetCriticalStrikeChance();
}

float AWeaponBase::GetWeaponCriticalStrikeMultiplier() const
{
	return WeaponDamageDetails.GetCriticalStrikeDamageMultiplier();
}

EDamageTypes AWeaponBase::GetWeaponDamageType() const
{
	return WeaponDamageDetails.GetDamageType();
}

FDamageEvent AWeaponBase::GetWeaponDamageEventClass() const
{
	return CurrentDamageEventClass;
}
