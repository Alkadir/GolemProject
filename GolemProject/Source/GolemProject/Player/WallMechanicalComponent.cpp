// Golem Project - Créajeux 2020


#include "WallMechanicalComponent.h"
#include "Engine/World.h"
#include "GolemProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Helpers/HelperLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UWallMechanicalComponent::UWallMechanicalComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWallMechanicalComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* owner = GetOwner();
	mCharacter = Cast<AGolemProjectCharacter>(owner);
	world = GetWorld();
}


// Called every frame
void UWallMechanicalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	/*if (NeedToRotate)
	{
		TimerRotation += DeltaTime * SpeedRotation;
		mCharacter->SetActorRotation(FMath::Lerp(mCharacter->GetActorRotation(), RotationToReach, TimerRotation));
		if (TimerRotation >= 1.0f)
		{
			NeedToRotate = false;
			TimerRotation = 0.0f;
		}
	}*/
}

bool UWallMechanicalComponent::WallJump()
{
	if (world && mCharacter)
	{
		FHitResult hitResult;
		FVector start(mCharacter->GetActorLocation() - mCharacter->GetActorUpVector() * 96.0f);
		FVector end(start - FVector::UpVector);
		//to change but don't know how for now
		if (UKismetSystemLibrary::BoxTraceSingle(world, start, end, FVector(DistanceFromWallToJump, DistanceFromWallToJump, 1.0f), FRotator::ZeroRotator, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, hitResult, true))
		{
			ActorsToIgnore.Empty();
			ActorsToIgnore.Add(hitResult.GetActor());
			DirectionJump = hitResult.Normal;
			RotationToReach = UKismetMathLibrary::MakeRotFromX(DirectionJump);
			DirectionJump.Z = 1.0f;
			mCharacter->LaunchCharacter(DirectionJump * 600.0f, true, true);
			NeedToRotate = true;
			return true;
		}
	}
	return false;
}

void UWallMechanicalComponent::ClearWallToIgnore()
{
	ActorsToIgnore.Empty();
}

