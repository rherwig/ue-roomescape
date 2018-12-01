// Fill out your copyright notice in the Description page of Project Settings.

#include "PositionReport.h"

#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UPositionReport::UPositionReport()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPositionReport::BeginPlay()
{
	Super::BeginPlay();

	const auto ObjectName = GetOwner()->GetName();
	auto Transform = GetOwner()->GetActorLocation();
	auto ObjectPosition = FString::Printf(TEXT("X: %f, Y: %f, Z: %f"), Transform.X, Transform.Y, Transform.Z);

	UE_LOG(LogTemp, Log, TEXT("Position report ready for component %s at position %s"), *ObjectName, *ObjectPosition);
}


// Called every frame
void UPositionReport::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

