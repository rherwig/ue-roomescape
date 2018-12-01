// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found input component"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Location;
	FRotator Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Location, OUT Rotation);

	const auto LineTraceEnd = Location + Rotation.Vector() * Reach;
	DrawDebugLine(GetWorld(), Location, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 5.f);

	const auto TraceParameters = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	FHitResult LineTraceHit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit,
		Location,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	if (LineTraceHit.GetActor() != nullptr) {
		const auto HitActorName = LineTraceHit.GetActor()->GetName();

		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *HitActorName);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed."));
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released."));
}
