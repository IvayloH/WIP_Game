// Copyright 2019 IvayloH. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "Game/GameEnums.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "CharacterBase.generated.h"

UCLASS(config = Game)
class ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMeleeAttackSignature, const bool, bIsAttacking);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FPostDamageTakenSignature, const float, InitialDamage, const float, ActualDamageTaken, const bool, bWasCriticalStrike, const bool, bWasDamageAbsorbed, const EDamageTypes, DamageType, const float, NewCurrentHealthPoints);
	/** Reference to the skeletal mesh component on the character.*/
	USkeletalMeshComponent* CharacterMeshReference;

	/** Timer handle for the dash mechanism.*/
	FTimerHandle DashHandle;

	/** Flag for whether the character will take damage when hit.
	TODO: This should be replaced with simply disabling the projectile detection so the projectiles go through instead of simply hitting the character and not applying damage.*/
	bool bCanCharacterTakeDamage : 1;

private:
	class ARangedWeaponBase* RangedWeapon;
	class AMeleeWeaponBase* MeleeWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterSettings|Movement")
	EPlayerMovementPassiveAbilities PlayerMovementPassiveAbility;

	/** The velocity to apply to the character when dashing while on the ground.*/
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSettings|Movement|Dash")
	float GroundDashVelocity;

	/** The velocity to apply to the character when dashing while in the air.*/
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSettings|Movement|Dash")
	float AirDashVelocity;

	/** Can the player perform a dash while jumping?*/
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSettings|Movement|Dash")
	bool bCanDashMidJump;

	/** The particle system that is used to create the dash effect.*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "CharacterSettings|Movement|Dash", meta = (AllowPrivateAccess = true))
	class UParticleSystemComponent* DashParticleSystem;

	UPROPERTY(EditInstanceOnly, Category = "CharacterSettings|Weapons|RangedWeapon")
	TArray<TSubclassOf<class ARangedWeaponBase>> RangedWeaponBlueprints;

	class ARangedWeaponBase* CurrentRangedWeapon;

	UPROPERTY(EditInstanceOnly, Category = "CharacterSettings|Weapons|MeleeWeapon")
	TArray<TSubclassOf<class AMeleeWeaponBase>> MeleeWeaponBlueprints;

	class AMeleeWeaponBase* CurrentMeleeWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterSettings|Weapons|MeleeWeapon")
	FName WeaponSocketName;

	/** Timer handle for the melee attack mechanism.*/
	FTimerHandle MeleeAttackTimerHandle;

	/** Flag to keep track of whether the player is currently melee attacking.*/
	bool bIsMeleeAttacking;

	/** The particle system that is used to create the weapon switch effect.*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "CharacterSettings|Weapons", meta = (AllowPrivateAccess = true))
	class UParticleSystemComponent* WeaponSwitchParticleEffect;

	UPROPERTY(EditInstanceOnly, Category = "CharacterSettings|Weapons", meta = (EditCondition = "!bSpawnCharacterWithRangedWeapon"))
	bool bSpawnCharacterWithMeleeWeapon;

	UPROPERTY(EditInstanceOnly, Category = "CharacterSettings|Weapons", meta = (EditCondition = "!bSpawnCharacterWithMeleeWeapon"))
	bool bSpawnCharacterWithRangedWeapon;


	/** By how much % to reduce incoming damage of a specific type.*/
	UPROPERTY(EditAnywhere, Category = "CharacterSettings|Health|DamageResistance")
	TMap <EDamageTypes, float> DamageTypeResistances;

	/** What chance is there for a specific damage type attack to be absorbed?*/
	UPROPERTY(EditAnywhere, Category = "CharacterSettings|Health|DamageResistance")
	TMap <EDamageTypes, float> DamageTypeAbsorptionChance;

	/** How long should a DoT of a specific damage type last for? */
	UPROPERTY(EditAnywhere, Category = "CharacterSettings|Health|DamageResistance")
	TMap <EDamageTypes, float> DamageOverTimeDurationByDamageType;


	/** Is the character able to climb up a ledge when jumping? */
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSettings|Movement")
	bool bCanCharacterClimbUpLedges;

	/** Trace channel for checking layers in front of the character. */
	ECollisionChannel const LedgeClimbingTraceChannel = ECC_GameTraceChannel8;
	
	/** How much should the start of the trace be offset by on the Z axis? Distance calculated using the mesh component as a starting point. */
	float LayerTraceStartOffsetZ;

	/** How long should the trace be? */
	float LayerTraceLength;

	/** Was the last trace for a layer a success or a failure? */
	bool bLastTraceHit;
	
	/** The direction the character was facing when the last trace for ledge climbing was successful. */
	FVector LedgeClimbingForwardDirection;

	/** If bLastTraceHit is TRUE, this will contain the information about the last trace hit. */
	FHitResult LastLayerTraceHitResult;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "CharacterSettings|Health|Effects")
	class UDamageOverTimeEffect* DamageOverTimeComponent;

	/** The maximum health points this actor should have.*/
	UPROPERTY(EditAnywhere, Category = "CharacterSettings|Health")
	float MaxHealth;

	/** The current health points this actor should start with.*/
	UPROPERTY(EditAnywhere, Category = "CharacterSettings|Health")
	float CurrentHealth;

public:
	UPROPERTY(BlueprintAssignable)
	FMeleeAttackSignature OnMeleeAttackStateChanged;

	UPROPERTY(BlueprintAssignable)
	FPostDamageTakenSignature OnPostDamageTaken;

protected:
	virtual void Jump() override;
	virtual void StopJumping() override;

	/** If we can dash, perform dash and start timer for dash end. **/
	virtual void BeginDash();
	virtual void EndDash();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION()
	virtual void BeginMeleeAttack();
	UFUNCTION()
	virtual void EndMeleeAttack();

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Character has died. Update any relevant logic.*/
	virtual void CharacterDeath();

	/**
	* Move the character to a position using the data from the last successful trace hit.
	* @param HitResult The last successful trace for a layer the character can stand on.
	*/
	void ClimbUpLedge(FHitResult HitResult);

public:
	/** Updates the player stats based on the item type. Called when the player gets into range of a pick up item.*/
	void OnItemPickedUp(EItemTypes PickedupItemType, float Amount);

	void OnItemPickedUp(EItemTypes PickedupItemType, UBlueprint Blueprint);
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	void TakeDamage(struct FDamageDetails const& DamageDetails, class AController* EventInstigator, AActor* DamageCauser);
	
	/** Returns the Max health value for this character.*/
	UFUNCTION(BlueprintCallable, Category = "Character")
	float GetMaxHealth() const;

	/** Returns the current health value for this character.*/
	UFUNCTION(BlueprintCallable, Category = "Character")
	float GetCurrentHealth() const;

	/** Heal this character by a specifc amount.*/
	UFUNCTION(BlueprintCallable)
	void Heal(float AmountToHeal);

	UFUNCTION(BlueprintCallable)
	/** Set a new damage type resistance value for a specific Damage Type.*/
	void UpdateDamageTypeResistance(EDamageTypes DamageTypeResistanceToModify, const float NewResistanceValue);

	UFUNCTION(BlueprintCallable)
	/** Retrieve the resistance value for a specific Damage Type from the TMap.*/
	float GetDamageTypeResistance(EDamageTypes DamageTypeToLookUp);

	UFUNCTION(BlueprintCallable)
	/** Retrieve the damage absorption chance for a specific Damage Type from the TMap.*/
	float GetDamageTypeAbsorptionChance(EDamageTypes DamageTypeToLookUp);

	UFUNCTION(BlueprintCallable)
	/** Retrieve the duration for a specific damage type DoT.*/
	float GetDamageOverTimeDurationByDamageType(EDamageTypes DamageTypeToLookUp);

	UFUNCTION(BlueprintCallable)
	/** Retrieve the current negative status effect that is affecting this character.*/
	ENegativeStatusEffects GetCurrentNegativeStatusEffect() const;

	/** Update the currently held weapon by destroying the old one and spawning a new one.*/
	void SpawnWeapon(TSubclassOf<class AWeaponBase> WeaponBlueprint);
};