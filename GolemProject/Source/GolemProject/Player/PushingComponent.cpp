// Golem Project - Créajeux 2020


#include "PushingComponent.h"

// Sets default values for this component's properties
UPushingComponent::UPushingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPushingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPushingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPushingComponent::PushBloc(FVector newPushingDirection, FVector newPushingPosition, FRotator newPushingRotation)
{
	isPushingObject = true;
	pushingDirection = newPushingDirection;
	pushingPosition = newPushingPosition;
	pushingRotation = newPushingRotation;
	OnPushingObject.Broadcast();
}

void UPushingComponent::StopPushBloc()
{
	isPushingObject = false;
	OnStopPushingObject.Broadcast();
}
