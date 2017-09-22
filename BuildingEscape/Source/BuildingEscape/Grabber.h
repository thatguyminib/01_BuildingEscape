// Copyright Cody Caro 2017

#pragma once

#include "Engine.h"
#include "OpenDoor.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGrabber);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// detect if we are looking at a key or not
	UPROPERTY(BlueprintAssignable)
	FGrabber InteractText;

	UPROPERTY(BlueprintAssignable)
	FGrabber InteractWithDoor;

	UPROPERTY(BlueprintAssignable)
	FGrabber NeedKey;

	UPROPERTY(BlueprintAssignable)
	FGrabber PickUpText;

	bool HasKey;

private:
	//How far ahead of the player can we reach in cm
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	
	UInputComponent* InputComp = nullptr;

	// Ray-cast and grab what's in reach
	void Grab();

	// Release object that is grabbed
	void Release();

	// Pick up the actual key
	void PickUpKey();

	// Find (assumed) Attached physics handle
	void FindPhysicsHandleComponent();

	// Setup (assumed) attached input component
	void SetupInputComponent();

	// return hit for first physics bods in reach
	const FHitResult GetFirstPhysicsBodyInReach();
	
	// Get key that is hit
	AActor* ActorHit;

	//Get door that is hit
	AActor* DoorActorHit;

	// detect if we are looking at a key or not
	void GetKeyInReach();

	// detect if we are looking at a door
	void GetDoorInReach();

	// Return current start of reach line
	FVector GetReachLineStart();

	// Return current end of reach line
	FVector GetReachLineEnd();
};
