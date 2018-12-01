// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (IsTimeToClose(GetWorld()->GetTimeSeconds()))
	{
		CloseDoor();
	}
}

void UOpenDoor::OpenDoor() const
{
	RotateDoor(OpenAngle);
}

void UOpenDoor::CloseDoor() const
{
	RotateDoor(0.f);
}

void UOpenDoor::RotateDoor(const float Yaw) const
{
	Owner->SetActorRotation(FRotator(0.f, Yaw, 0.f));
}

bool UOpenDoor::IsTimeToClose(const float Time) const
{
	return Time - LastDoorOpenTime >= DoorCloseDelay;
}

