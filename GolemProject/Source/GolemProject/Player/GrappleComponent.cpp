// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GolemProjectCharacter.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values for this component's properties
UGrappleComponent::UGrappleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrappleComponent::BeginPlay()
{
	Super::BeginPlay();

	mPawn = Cast<AGolemProjectCharacter>(GetOwner());
	bIsGrappling = false;
}


void UGrappleComponent::GoToDestination()
{
	FVector direction = FVector::ZeroVector;
	UWorld* world = GetWorld();

	if (world && mPawn)
	{
		APlayerController* PlayerController = Cast<APlayerController>(mPawn->GetController());

		FHitResult TraceResult;

		if (PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, TraceResult))
		{
			mDestination = TraceResult.Location;
			bIsGrappling = true;
		}
	}

}

// Called every frame
void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsGrappling)
	{
		if (mPawn)
		{
			mDirection = mDestination - mPawn->GetActorLocation();
			float dist = mDirection.Size();

			mDirection = (mDestination - mPawn->GetActorLocation()).GetSafeNormal();

			if (dist > offsetStop)
			{
				mPawn->LaunchPawn(mDirection * velocity, false, false);
			}
			else
			{
				AController* ctrl = mPawn->GetController();

				if (ctrl)
				{
					ACharacter* character = ctrl->GetCharacter();
					if (character)
					{
						character->GetCharacterMovement()->Velocity *= 0.5f;
					}
				}
				bIsGrappling = false;
			}
		}
	}
}
