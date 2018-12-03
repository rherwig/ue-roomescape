// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "PickupItem.h"
#include "GameFramework/Actor.h"
#include <vector>
#include <memory>

#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMESCAPE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int GetInventorySize();

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory", BlueprintCallable, meta = (DisplayName = "Pupu Event"))
	void Pupu();

private:
	UPROPERTY(EditAnywhere)
	float Reach = 100.f;

	short CurrentInventorySlot = 0;

	AActor* Owner = nullptr;

	UInputComponent* InputComponent = nullptr;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<UPickupItem*> GetInventoryItems();
	

	std::vector<UPickupItem*> Inventory;
	void DisableActor(AActor* Actor) const;
	void EnableActor(AActor* Actor) const;
	void SetupInputComponent();
	void HandleAddItemToInventory();
	void HandleRemoveItemFromInventory();
	void HandleInventoryUp();
	void HandleInventoryDown();
	FHitResult GetPhysicsBodyHitResult() const;

};
