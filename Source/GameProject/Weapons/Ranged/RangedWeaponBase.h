// Copyright 2019 IvayloH. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "RangedWeaponBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT_API ARangedWeaponBase : public AWeaponBase
{
	GENERATED_BODY()

public:
	// Default Constructor
	ARangedWeaponBase();

protected: // Properties
	UPROPERTY(VisibleDefaultsOnly, Category = "Properties")
	class USkeletalMeshComponent* WeaponSkeletalMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Properties")
	class USceneComponent* MuzzleLocation;

	FDamageDetails DamageDetails;

protected: // Functions
	virtual void BeginPlay() override;

public: // Properties
	/** The projectile class to spawn. */
	UPROPERTY(EditDefaultsONly, Category = "ProjectileSettings")
	TSubclassOf<class AProjectileBase> ProjectileClass;

	/** Gun muzzle's offset from the characters location. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
	FVector GunOffset;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSettings")
	class USoundBase* WeaponFireSound;

public: //Functions
	/** Fire a projectile.*/
	void OnWeaponFire();

	UFUNCTION(BlueprintCallable)
	void SetWeaponDamageType(EDamageTypes NewDamageType);
};
