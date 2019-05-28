// Copyright 2019 IvayloH. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/GameStructs.h"
#include "ProjectileBase.generated.h"

UCLASS()
class GAMEPROJECT_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

private: // Properties
	FDamageDetails DamageDetails;
	FWeaponDamageDetails WeaponDamageDetails;

protected: // Properties
	UPROPERTY(VisibleDefaultsOnly, Category = "Properties")
	class UStaticMeshComponent* ProjectileMeshComponent;

	/** Sphere used for collision detecting. E.g. hitting actors/walls*/
	UPROPERTY(VisibleDefaultsOnly, Category = "Properties")
	class USphereComponent* CollisionSphereComponent;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	AActor* Instantiator;
	AController* InstantiatorController;

protected: // Functions
	void BeginPlay() override;

	/** Called when the projectile hits something. */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	void SetupProjectileProperties(FWeaponDamageDetails const& WeaponDamageDetails, AController* const InstantiatorController, AActor* const Instantiator);
};
