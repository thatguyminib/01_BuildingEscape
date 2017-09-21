// Copyright Cody Caro 2017

#pragma once

#include "Engine.h"
#include "Components/ActorComponent.h"
#include "OpenSecretWall.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOpenSecretWall);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenSecretWall : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenSecretWall();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOpenSecretWall OnOpen;

	UPROPERTY(BlueprintAssignable)
	FOpenSecretWall OnClose;

private:

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float TriggerMass = 30.f;

	// The owning wall
	AActor* Owner = nullptr;

	// returns total mass in kg
	float GetTotalMassOfActorsOnPlate();

		
	
};
