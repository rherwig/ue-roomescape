// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "InventoryComponent.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	SetupInputComponent();
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int UInventoryComponent::GetInventorySize()
{
	return Inventory.size();
}

//void UInventoryComponent::Pupu()
//{
//	UE_LOG(LogTemp, Warning, TEXT("Add event called"));
//}


TArray<UPickupItem*> UInventoryComponent::GetInventoryItems()
{
	TArray<UPickupItem*> InventoryItems;
	for (auto Item : Inventory)
	{
		InventoryItems.Add(Item);
	}
	return InventoryItems;
}

void UInventoryComponent::DisableActor(AActor* Actor) const
{
	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
	Actor->SetActorTickEnabled(false);
}

void UInventoryComponent::EnableActor(AActor* Actor) const
{
	Actor->SetActorHiddenInGame(false);
	Actor->SetActorEnableCollision(true);
	Actor->SetActorTickEnabled(true);
}


void UInventoryComponent::HandleAddItemToInventory()
{
	UE_LOG(LogTemp, Log, TEXT("Picking up Item..."));

	auto HitResult = GetPhysicsBodyHitResult();
	const auto Actor = HitResult.GetActor();

	if (!Actor) {
		UE_LOG(LogTemp, Warning, TEXT("No actor in grab range."));
		return;
	}

	const auto Component = Actor->FindComponentByClass<UPickupItem>();
	if (!Component)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Found pickup component for actor %s"), *Actor->GetName());

	Inventory.push_back(Component);
	DisableActor(Actor);
	this->Pupu();
}


void UInventoryComponent::HandleRemoveItemFromInventory()
{
	if (Inventory.empty())
	{
		UE_LOG(LogTemp, Error, TEXT("No Item in Inventory"));
		return;
	}

	const auto InventoryItem = Inventory[CurrentInventorySlot];
	const auto Actor = InventoryItem->GetOwner();

	Actor->SetActorLocation(Owner->GetActorLocation());
	EnableActor(Actor);

	Inventory.erase(Inventory.begin() + CurrentInventorySlot);
	HandleInventoryDown();
}

void UInventoryComponent::HandleInventoryDown()
{
	if (CurrentInventorySlot == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't scroll down past index 0!"));
		return;
	}

	CurrentInventorySlot--;
	UE_LOG(LogTemp, Warning, TEXT("Inventory slot: %d"), CurrentInventorySlot);
}

void UInventoryComponent::HandleInventoryUp()
{
	if (CurrentInventorySlot == static_cast<short>(Inventory.size()) - 1 || Inventory.empty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't scroll up past last index!"));
		return;
	}

	CurrentInventorySlot++;
	UE_LOG(LogTemp, Warning, TEXT("Inventory slot: %d"), CurrentInventorySlot);
}


void UInventoryComponent::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	InputComponent->BindAction("Pickup Item", IE_Pressed, this, &UInventoryComponent::HandleAddItemToInventory);
	InputComponent->BindAction("Drop Item", IE_Pressed, this, &UInventoryComponent::HandleRemoveItemFromInventory);
	InputComponent->BindAction("Inventory Up", IE_Pressed, this, &UInventoryComponent::HandleInventoryUp);
	InputComponent->BindAction("Inventory Down", IE_Pressed, this, &UInventoryComponent::HandleInventoryDown);
}


FHitResult UInventoryComponent::GetPhysicsBodyHitResult() const
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	const auto LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;
	const auto TraceParameters = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	FHitResult LineTraceHit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit,
		PlayerViewpointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECC_PhysicsBody),
		TraceParameters
	);

	return LineTraceHit;
}