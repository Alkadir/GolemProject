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
	CanAim = true;
	NeedToRotate = false;
	IsWallJumping = false;
	CanWallJump = true;
	timerWallJump = CDWallJump;
}


// Called every frame
void UWallMechanicalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	timerWallJump -= DeltaTime;
	if (IsWallJumping)
	{
		mCharacter->SetActorLocation(PositionFreeze);
	}
	if (NeedToRotate)
	{
		mCharacter->SetActorRotation(FMath::Lerp(InitialRotation, RotationToReach, SpeedRotation));
		InitialRotation = mCharacter->GetActorRotation();
		if (InitialRotation.Equals(RotationToReach, 5.0f))
		{
			NeedToRotate = false;
			CanAim = true;
			CanWallJump = true;
			EndJump.Broadcast();
		}
	}
}

bool UWallMechanicalComponent::WallJump()
{
	if (world && mCharacter && timerWallJump <= 0.f)
	{
		FHitResult hitResult;
		FHitResult hitResult2;
		FVector forward = mCharacter->GetActorForwardVector();
		for (float i = -30.0f; i <= 30.0f; i += 5.0f)
		{
			FVector dir = UKismetMathLibrary::RotateAngleAxis(forward, i, FVector::UpVector);
			FVector start(mCharacter->GetActorLocation() - mCharacter->GetActorUpVector() * 96.0f);
			FVector end(start + dir * DistanceFromWallToJump);
			if (world->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility))
			{
				FVector endBehind(hitResult.ImpactPoint + hitResult.Normal * DistanceBehind);
				if (!world->LineTraceSingleByChannel(hitResult2, hitResult.ImpactPoint, endBehind, ECC_Visibility))
				{
					ActorsToIgnore.Empty();
					ActorsToIgnore.Add(hitResult.GetActor());
					DirectionJump = hitResult.Normal;
					PositionFreeze = mCharacter->GetActorLocation();
					RotationToReach = UKismetMathLibrary::MakeRotFromX(DirectionJump);
					DirectionJump.Z = HeightJump;
					IsWallJumping = true;
					mCharacter->GetMovementComponent()->Deactivate();
					InitialRotation = mCharacter->GetActorRotation();
					CanAim = false;
					CanWallJump = false;
					timerWallJump = CDWallJump;
					return true;
				}
			}
		}
	}
	return false;
}

void UWallMechanicalComponent::LaunchForWallJump()
{
	mCharacter->GetMovementComponent()->Activate();
	mCharacter->LaunchCharacter(DirectionJump * ForceJump, true, true);
	NeedToRotate = true;
	IsWallJumping = false;
}

void UWallMechanicalComponent::ClearWallToIgnore()
{
	ActorsToIgnore.Empty();
}

