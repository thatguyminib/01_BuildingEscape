// Copyright Cody Caro 2017


#include "Grabber.h"
#include "Gameframework/Actor.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Missing Physics Handler from %s"), *(GetOwner()->GetName()))
	}
	
}

/// Look for attached input component (only appears at runtime)
void UGrabber::SetupInputComponent()
{

	InputComp = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComp) {
		InputComp->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComp->BindAction("Grab", IE_Released, this, &UGrabber::Release);
		InputComp->BindAction("Key", IE_Pressed, this, &UGrabber::PickUpKey);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Missing Input Component from %s"), *(GetOwner()->GetName()))
	}
}



void UGrabber::Grab()
{
	// Line trace and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); //Gets the mesh in our case
	auto ActorHit = HitResult.GetActor();

	/// if we hit something then attach a physics handle
	if (ActorHit != nullptr) {
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent
		(
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::PickUpKey()
{
	if (!ActorHit) { return; }
	HasKey = true;
	//UOpenDoor().OnOpen.Broadcast();
	ActorHit->Destroy();
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		// move the object we are holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
	GetKeyInReach();
	GetDoorInReach();
	auto HitObject = GetFirstPhysicsBodyInReach();
	if (HitObject.GetActor()) {
		PickUpText.Broadcast();
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	
	/// Line-trace (AKA Ray-cast) out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResult;
}

void UGrabber::GetKeyInReach()
{
	/// Line-trace out and see if we are hitting a key
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel1),
		TraceParameters
	);

	ActorHit = HitResult.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Error, TEXT("Hitting %s"), *(ActorHit->GetName()))
		InteractText.Broadcast();
	}
	
}

void UGrabber::GetDoorInReach()
{
	/// Line-trace out and see if we are hitting a key
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel2),
		TraceParameters
	);

	DoorActorHit = HitResult.GetActor();
	if (DoorActorHit && HasKey) {
		UE_LOG(LogTemp, Error, TEXT("Hitting %s"), *(DoorActorHit->GetName()))
			InteractWithDoor.Broadcast();
	}
	else if (DoorActorHit && !HasKey) {
		NeedKey.Broadcast();
	}

}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

