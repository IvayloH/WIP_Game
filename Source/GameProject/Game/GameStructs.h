// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapons/DamageTypes/PhysicalDamageType.h"
#include "Weapons/DamageTypes/IceDamageType.h"
#include "Weapons/DamageTypes/FireDamageType.h"
#include "Weapons/DamageTypes/EnergyDamageType.h"
#include "Weapons/DamageTypes/ElectricDamageType.h"
#include "Weapons/DamageTypes/AcidDamageType.h"
#include "Game/GameEnums.h"
#include "GameStructs.generated.h"

USTRUCT(BlueprintType)
struct FWeaponDamageDetails
{
	GENERATED_BODY()
private:
	/** The type of damage this weapon deals.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	EDamageTypes DamageType;

	/** How much base damage should this weapon when dealing damage? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", UIMin = "0.0"))
	float BaseDamage;

	/** What is the chance the weapon deals a critical strike when dealing damage? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float CriticalStrikeChance;

	/** By how much should the base damaged be increased when this weapon deals a critical strike?*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true", ClampMin = "1.0", UIMin = "1.0"))
	float CriticalStrikeDamageMultiplier;

	/** What is the chance the weapon applies a Damage Over Time effect when dealing damage? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float ChanceToInflictDot;

public:
	FWeaponDamageDetails()
	{
		this->DamageType = EDamageTypes::Physical;
		this->BaseDamage = .0f;
		this->CriticalStrikeChance = .0f;
		this->CriticalStrikeDamageMultiplier = 1.0f;
		this->ChanceToInflictDot = .0f;
	}

	FWeaponDamageDetails(EDamageTypes const DamageType, float const BaseDamage, float const CriticalStrikeChance, float const CriticalStrikeDamageMultiplier, float const ChanceToInflictDot)
	{
		this->DamageType = DamageType;
		this->BaseDamage = BaseDamage;
		this->CriticalStrikeChance = CriticalStrikeChance;
		this->CriticalStrikeDamageMultiplier = CriticalStrikeDamageMultiplier;
		this->ChanceToInflictDot = ChanceToInflictDot;
	}

	EDamageTypes GetDamageType() const { return DamageType; }
	float GetBaseDamage() const { return BaseDamage; }
	float GetCriticalStrikeChance() const { return CriticalStrikeChance; }
	float GetCriticalStrikeDamageMultiplier() const { return CriticalStrikeDamageMultiplier; }
	float GetChanceToInflictDot() const { return ChanceToInflictDot; }

	/**
	* Sets the Damage Type.
	* @return True, if damage type has changed.
	*/
	bool SetDamageEvent(EDamageTypes NewDamageType)
	{
		if (DamageType != NewDamageType)
		{
			DamageType = NewDamageType;
			return true;
		}
		return false;
	}

	/** Returns the equivalent Damage Event of the currently set Damage Type. */
	FDamageEvent GetEquivalentDamageEvent() const
	{
		switch (DamageType)
		{
		case EDamageTypes::Physical:
			return FDamageEvent(TSubclassOf<UPhysicalDamageType>(UPhysicalDamageType::StaticClass()));

		case EDamageTypes::Fire:
			return FDamageEvent(TSubclassOf<UFireDamageType>(UFireDamageType::StaticClass()));

		case EDamageTypes::Ice:
			return FDamageEvent(TSubclassOf<UIceDamageType>(UIceDamageType::StaticClass()));

		case EDamageTypes::Electric:
			return FDamageEvent(TSubclassOf<UElectricDamageType>(UElectricDamageType::StaticClass()));

		case EDamageTypes::Energy:
			return FDamageEvent(TSubclassOf<UEnergyDamageType>(UEnergyDamageType::StaticClass()));

		default:
			return FDamageEvent(TSubclassOf<UAcidDamageType>(UAcidDamageType::StaticClass()));
		}
	}
};

USTRUCT()
struct FDamageDetails
{
	GENERATED_BODY()
private:
	/** Amount of damage we want to deal. */
	float DamageAmount;

	/** Whether this is a critical strike. */
	bool bIsCriticalStrike;

	/** What is the chance of this strike to apply a Dot effect? */
	float ChanceToInflictDot;

	/** What is the damage type of this attack? */
	struct FDamageEvent DamageEvent;

public:
	FDamageDetails()
	{
		this->DamageAmount = 0.f;
		this->bIsCriticalStrike = false;
		this->ChanceToInflictDot = 0.f;
		this->DamageEvent = FDamageEvent(TSubclassOf<UPhysicalDamageType>(UPhysicalDamageType::StaticClass()));
	}

	FDamageDetails(float DamageAmount, bool bIsCriticalStrike, float ChanceToInflictDot, struct FDamageEvent const& DamageEvent)
	{
		this->DamageAmount = DamageAmount;
		this->bIsCriticalStrike = bIsCriticalStrike;
		this->ChanceToInflictDot = ChanceToInflictDot;
		this->DamageEvent = DamageEvent;
	}

	float GetDamageAmount() const { return DamageAmount; }
	bool IsCriticalStrike() const { return bIsCriticalStrike; }
	float GetChanceToInflictDot() const { return ChanceToInflictDot; }
	FDamageEvent GetDamageEvent() const { return DamageEvent; }
};