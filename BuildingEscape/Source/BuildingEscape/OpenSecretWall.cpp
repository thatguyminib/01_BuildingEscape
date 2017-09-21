// Copyright Cody Caro 2017

#include "OpenSecretWall.h"
#include "Gameframework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenSecretWall::UOpenSecretWall()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenSecretWall::BeginPlay()
{
	Super::BeginPlay();
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Missing Pressure Plate Trigger from %s"), *(GetOwner()->GetName()))
	}
	Owner = GetOwner();
	
}


// Called every frame
void UOpenSecretWall::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) {
		OnOpen.Broadcast();
	}
	else {
		OnClose.Broadcast();
	}
}

float UOpenSecretWall::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	//find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// iterate through them adding their masses
	for (const auto* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp,Warning, TEXT("%s on pressure plate"), *Actor->GetName())
	}

	return TotalMass;
}

