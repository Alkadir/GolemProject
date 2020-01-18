// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GolemProjectCharacter.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Helpers/HelperLibrary.h"
#include "Camera/CameraComponent.h"
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
	AActor* owner = GetOwner();
	mCharacter = Cast<AGolemProjectCharacter>(owner);

	UChildActorComponent* child = HelperLibrary::GetComponentByName<UChildActorComponent>(mCharacter, "ShoulderCamera");
	mCamera = HelperLibrary::GetComponentByName<UCameraComponent>(child->GetChildActor(), "Camera");

	bIsGrappling = false;
}


void UGrappleComponent::GoToDestination()
{
	FVector direction = FVector::ZeroVector;
	UWorld* world = GetWorld();

	if (world && mCamera)
	{
		FHitResult hitResult;

		if (world->LineTraceSingleByChannel(hitResult, mCamera->GetComponentLocation(), mCamera->GetForwardVector() * maxDistance, ECollisionChannel::ECC_Visibility))
		{
			mDestination = hitResult.Location;
			bIsGrappling = true;
			mCharacter->GetCharacterMovement()->GroundFriction = 0.0f;
		}
	}

}

// Called every frame
void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsGrappling)
	{
		if (mCharacter)
		{
			mDirection = mDestination - mCharacter->GetActorLocation();
			float dist = mDirection.Size();

			mDirection = (mDestination - mCharacter->GetActorLocation()).GetSafeNormal();

			if (dist > offsetStop)
			{
				mCharacter->LaunchCharacter(mDirection * velocity, false, false);
				mDirection.Z = 0.0f;
				mCharacter->SetActorRotation(mDirection.Rotation());
			}
			else
			{
				//Find destination stop player
				AController* ctrl = mCharacter->GetController();

				if (ctrl)
				{
					if (mCharacter)
					{
						mCharacter->GetCharacterMovement()->Velocity *= 0.15f;
						bIsGrappling = false;
						mCharacter->ResetFriction();
					}
				}
				
			}
		}
	}
}
