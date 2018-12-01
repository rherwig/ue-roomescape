#include "Grabber.h"

#define OUT

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
}


void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics component not found."));
	}
}


void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Input component not found."));
		return;
	}

	InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
}


void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		FVector PlayerViewpointLocation;
		FRotator PlayerViewpointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

		const auto LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;

		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}


void UGrabber::Grab()
{
	auto HitResult = GetPhysicsBodyHitResult();
	if (!HitResult.GetActor()) {
		UE_LOG(LogTemp, Warning, TEXT("No actor in grab range."));
		return;
	}

	const auto ComponentToGrab = HitResult.GetComponent();
	const auto ComponentOwner = ComponentToGrab->GetOwner();

	PhysicsHandle->GrabComponentAtLocationWithRotation(
		ComponentToGrab, 
		NAME_None, 
		ComponentOwner->GetActorLocation(), 
		ComponentOwner->GetActorRotation()
	);
}


void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}


FHitResult UGrabber::GetPhysicsBodyHitResult() const
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
