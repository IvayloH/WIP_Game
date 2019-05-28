// Copyright 2019 IvayloH. All rights reserved.
#pragma once

#include "GameEnums.generated.h"

UENUM(BlueprintType)
enum class EPlayerMovementPassiveAbilities : uint8
{
	None,
	SingleJump,
	DoubleJump
};

UENUM(BlueprintType)
enum class EItemTypes : uint8
{
	// These will be updated once a final decision has been made
	Health, 
	Mana,
	Shield
};

UENUM(BlueprintType)
enum class EGadgetTypes : uint8
{
	// These will be updated once a final decision has been made
	ExampleGadget 
};

UENUM(BlueprintType)
enum class EWeaponTypes : uint8
{
	// These will be updated once a final decision has been made
	Pistol, 
	Rifle
};

UENUM(BlueprintType)
enum class EWeaponAttachementTypes : uint8
{
	// These will be updated once a final decision has been made
	WeaponAttachment
};

UENUM(BlueprintType)
enum class EAmmoTypes : uint8
{
	RegularAmmo,
	FireAmmo,
	IceAmmo,
	EnergyAmmo,
	ElectricAmmo,
	AcidAmmo
};

UENUM(BlueprintType)
enum class EDamageTypes : uint8
{
	Physical,
	Fire,
	Ice,
	Energy,
	Electric,
	Acid
};
// Keep track of enum count of EDamageTypes
static int32 DamageTypesEnumSize = 6;

UENUM(BlueprintType)
enum class ENegativeStatusEffects : uint8
{
	None,
	Bleeding,
	Burning,
	Freezing,
	Electrified,
	Slowed,
	Corroding
};
static int32 NegativeStatusEffectsEnumSize = 7;