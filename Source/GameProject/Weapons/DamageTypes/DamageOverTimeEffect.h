// Copyright 2019 IvayloH. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/GameStructs.h"
#include "Characters/CharacterBase.h"
#include "DamageOverTimeEffect.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT_API UDamageOverTimeEffect : public UActorComponent
{
	GENERATED_BODY()

public:
	UDamageOverTimeEffect();

private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNegativeStatusEffectChanged, const ENegativeStatusEffects, NewNegativeStatusEffect);

	FDamageEvent DamageEventType;

protected:
	/** How often to deal damage when DoT is active? Value of 1.0 means deal damage every 1.0 seconds.*/
	UPROPERTY(EditDefaultsOnly, SaveGame, Category = "DamageSettings",
		meta = (ClampMin = "0.0", ClampMax = "60.0", UIMin = "0.0", UIMax = "60.0"))
	float DamageRate;

	/** How much damage to deal each time?*/
	UPROPERTY(EditDefaultsOnly, SaveGame, Category = "DamageSettings")
	float DamageOnTick;

	/** What is the chance of dealing critical damage each time damage is dealt?*/
	UPROPERTY(EditDefaultsOnly, SaveGame, Category = "DamageSettings",
		meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float CriticalStrikeChance;

	/** By how much to multiply the damage the actor will receive when it's a critical strike?*/
	UPROPERTY(EditDefaultsOnly, SaveGame, Category = "DamageSettings",
		meta = (ClampMin = "1.0", UIMin ="1.0"))
	float CriticalStrikeMultiplier;

	UPROPERTY(VisibleAnywhere, Category = "DamageSettings")
	ENegativeStatusEffects CurrentNegativeStatusEffect;

	FTimerHandle DamageDealingTimerHandle;

	ACharacterBase* OwningActor;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FNegativeStatusEffectChanged OnNegativeStatusEffectChanged;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/** Activate the components tick and begin to deal damage to the owning actor on each tick.*/
	void BeginDealingDamage(struct FDamageEvent const& DamageEvent, const float DamageEffectDuration);
	
	/** Stop the components tick and stop dealing damage to the owning actor.*/
	void EndDealingDamage();

	UFUNCTION(BlueprintCallable)
	/** Sets the damage rate at which this component deals damage when active. Value is clamped between 0.0f and 60.f*/
	void SetDamageRate(const float NewDamageRate);

	UFUNCTION(BlueprintCallable)
	/** Sets the damage that is done each time this component deals damage when active. Values clamped 0-100.0f.*/
	void SetDamageOnTick(float NewDamageOnTick);

	UFUNCTION(BlueprintCallable)
	/** Sets the chance this component has of dealing critical damage.*/
	void SetCriticalDamageChance(float NewCriticalDamageChance);

	UFUNCTION(BlueprintCallable)
	/** Returns the interval rate at which this component deals damage when it's active.*/
	float GetDamageRate() const;

	UFUNCTION(BlueprintCallable)
	/** Returns the Damage that is dealt every time the component deals damage to an Actor.*/
	float GetDamageOnTick() const;

	UFUNCTION(BlueprintCallable)
	/** Returns the critical damage chance this component has of dealing extra damage to the Actor every time it deals damage.*/
	float GetCriticalDamageChance() const;

	UFUNCTION(BlueprintCallable)
	/** Returns the critical damage multiplier value used to calculate critical damage values.*/
	float GetCriticalDamageMultiplier() const;

	/** Sets the Current Negative Status and calls the delegate.*/
	void SetNegativeStatusEffect(ENegativeStatusEffects NewNegativeStatusEffect);

	void SetNegativeStatusEffect(struct FDamageEvent const& NewDamageType);

	UFUNCTION(BlueprintCallable)
	ENegativeStatusEffects GetCurrentNegativeStatusEffect() const;
};
