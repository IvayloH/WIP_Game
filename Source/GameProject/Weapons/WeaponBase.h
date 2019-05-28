// Copyright 2019 IvayloH. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/GameStructs.h"
#include "WeaponBase.generated.h"

UCLASS()
class GAMEPROJECT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

private:
	FDamageEvent CurrentDamageEventClass;

protected:
	UPROPERTY(EditDefaultsOnly)
	FWeaponDamageDetails WeaponDamageDetails;

protected:
	virtual void BeginPlay() override;

public:
	/** Returns the damage that this weapon is currently set to deal.*/
	UFUNCTION(BlueprintCallable)
	float GetBaseWeaponDamage() const;

	UFUNCTION(BlueprintCallable)
	/** Returns the chance this weapon has to deal a critical strike when dealing damage. */
	float GetWeaponCrticalStrikeChance() const;

	UFUNCTION(BlueprintCallable)
	/** Returns the value by how much the base damage is increased by when dealing a critical strike. */
	float GetWeaponCriticalStrikeMultiplier() const;

	UFUNCTION(BlueprintCallable)
	/** Returns the chance (0-100) of inflicting a DoT when dealing damage.*/
	float GetChanceToInflictDot() const;

	UFUNCTION(BlueprintCallable)
	/** Returns the damage type for this weapon.*/
	EDamageTypes GetWeaponDamageType() const;

	/** Returns the damage event type for this weapon.*/
	FDamageEvent GetWeaponDamageEventClass() const;
};
