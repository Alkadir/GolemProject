// Golem Project - Créajeux 2020


#include "PushingComponent.h"
#include "GolemProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Objects/PushableBloc.h"

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
	if (AGolemProjectCharacter* character = Cast<AGolemProjectCharacter>(GetOwner()))
	{
		characterMovementCmpt = character->GetCharacterMovement();
	}
	// ...

}


// Called every frame
void UPushingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPushingComponent::GetIsPushingObject(bool startingToo)
{
	if (startingToo)
	{
		return isPushingObject || isStartingPushingObject;
	}
	else
	{
		return isPushingObject;
	}
}

void UPushingComponent::SetBlock(APushableBloc* _block)
{
	block = _block;
	if (block != nullptr)
	{
		sizeBlock = block->GetSizeBlock();
	}
}

void UPushingComponent::SetIsStartingPushingObject(bool isStartingPushing)
{
	isStartingPushingObject = isStartingPushing;
	if (isStartingPushing == true)
	{
		OnStartStartingPushingObject.Broadcast();
	}
	else
	{
		OnEndStartingPushingObject.Broadcast();
	}
}

void UPushingComponent::StartPushingObject()
{
	isPushingObject = true;
	OnStartPushingObject.Broadcast();
	if (characterMovementCmpt)
	{
		lastSpeed = characterMovementCmpt->MaxWalkSpeed;
		characterMovementCmpt->MaxWalkSpeed = speedWhenPushing;
	}
}

void UPushingComponent::EndPushingObject()
{
	isPushingObject = false;
	OnEndPushingObject.Broadcast();
	if (characterMovementCmpt)
	{
		characterMovementCmpt->MaxWalkSpeed = lastSpeed;
	}
}

void UPushingComponent::StartStartingPushingObject()
{
	isStartingPushingObject = true;
	OnStartStartingPushingObject.Broadcast();
}

void UPushingComponent::EndStartingPushingObject()
{
	isStartingPushingObject = false;
	OnEndStartingPushingObject.Broadcast();
	StartPushingObject();
}

void UPushingComponent::PushBloc(FVector newPushingDirection, FVector newPushingPosition, FRotator newPushingRotation)
{
	pushingDirection = newPushingDirection;
	pushingPosition = newPushingPosition;
	pushingRotation = newPushingRotation;
	StartStartingPushingObject();
}

void UPushingComponent::StopPushBloc()
{
	EndPushingObject();
}
