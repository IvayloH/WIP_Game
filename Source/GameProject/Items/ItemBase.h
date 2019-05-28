// Copyright 2019 IvayloH. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Game/GameEnums.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class USphereComponent;

UCLASS()
class GAMEPROJECT_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Settings")
	EItemTypes PickUpItemType;

	/** The collision for the item. It will be used to stop it from falling through the ground etc.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* CollisionBoundsBox;

	/** The model for the item.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* ObjectMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//TODO - replace with OnHit event instead of Overlap events to improve performance 
	//UFUNCTION()
	//void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
