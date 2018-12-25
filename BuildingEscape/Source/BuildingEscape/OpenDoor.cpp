// Copyright ScaryPowerfulGames


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#include "Components/PrimitiveComponent.h"
#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.0));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, 180, 0.0));
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// poll trigger volume
	if (GetTotalMassOfActorsOnPlate() > 30.f) // Todo make into a parameter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door

	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}

}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all overlapping actors
	TArray<AActor*> OverLappingActors;
	PressurePlate->GetOverlappingActors(OUT OverLappingActors);

	// Iterate through them adding their masses
	for (const auto& Actor : OverLappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName())
		//UE_LOG(LogTemp, Warning, TEXT("Pressure plate is unavailable on %s!"), GetOwner()->GetName)
	}

	return TotalMass;
}

