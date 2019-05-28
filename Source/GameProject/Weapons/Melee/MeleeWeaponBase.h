// Copyright 2019 IvayloH. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "Game/GameEnums.h"
#include "MeleeWeaponBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT_API AMeleeWeaponBase : public AWeaponBase
{
	GENERATED_BODY()

public:	
	AMeleeWeaponBase();

protected:
	/** How long does the weapon attack animation last?*/
	UPROPERTY(EditDefaultsOnly, Category = "WeaponProperties")
	float WeaponAttackDuration;

	/** Range in which the weapon will deal damage when it overlaps another actor.*/
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class UCapsuleComponent* CollisionCapsule;

	/** The model for this weapon.*/
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class USkeletalMeshComponent* WeaponSkeletalMeshComponent;

protected:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	float GetWeaponAttackDuration() const;

	/** Returns a reference to the static mesh component.*/
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetSkeletalMeshComponent() const;

	/** Enable the overlap event generation on the collision capsule for this weapon.*/
	virtual void EnableWeaponOverlapEvents();

	/** Disable the overlap event generation on the collision capsule for this weapon.*/
	virtual void DisableWeaponOverlapEvents();
};