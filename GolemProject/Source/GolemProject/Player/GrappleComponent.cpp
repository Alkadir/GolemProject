// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GolemProjectCharacter.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Helpers/HelperLibrary.h"
#include "Camera/CameraComponent.h"
#include "Classes/Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/ProjectileHand.h"

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
	mSkeletalMesh = mCharacter->GetMesh();
	mIdBone = mSkeletalMesh->GetBoneIndex("hand_r");
	UChildActorComponent* child = HelperLibrary::GetComponentByName<UChildActorComponent>(mCharacter, "ShoulderCamera");
	mCamera = HelperLibrary::GetComponentByName<UCameraComponent>(child->GetChildActor(), "Camera");
}


void UGrappleComponent::GoToDestination()
{
	if (!currentProjectile)
	{
		UWorld* world = GetWorld();

		if (world && mCamera)
		{
			FHitResult hitResult;

			if (world->LineTraceSingleByChannel(hitResult, mCamera->GetComponentLocation(), mCamera->GetForwardVector() * maxDistance, ECollisionChannel::ECC_Visibility))
			{
				mSkeletalMesh->HideBone(mIdBone, EPhysBodyOp::PBO_None);

				currentProjectile = world->SpawnActor<AProjectileHand>(handProjectileClass, mSkeletalMesh->GetBoneTransform(mIdBone));
				if (currentProjectile)
				{
					FVector direction = (hitResult.Location - currentProjectile->GetActorLocation()).GetSafeNormal();
					mDestination = hitResult.Location;
					currentProjectile->Instigator = mCharacter->GetInstigator();
					currentProjectile->SetOwner(mCharacter);
					currentProjectile->LaunchProjectile(direction);
				}
			}
		}
	}
}

void UGrappleComponent::SetIKArm(FVector& _lookAt, bool& _isBlend)
{
	_lookAt = IKposition;
	_isBlend = (mCharacter->GetSightCameraEnabled() || currentProjectile);
}

void UGrappleComponent::UpdateIKArm()
{
	UWorld* world = GetWorld();

	if (world && mCamera)
	{
		FHitResult hitResult;
		if (world->LineTraceSingleByChannel(hitResult, mCamera->GetComponentLocation(), mCamera->GetForwardVector() * maxDistance, ECollisionChannel::ECC_Visibility))
		{

			mDirection = hitResult.Location - mCharacter->GetActorLocation();
			IKposition = hitResult.Location;
			mDirection.Z = 0.0f;
			mCharacter->SetActorRotation(mDirection.Rotation());

			//I don't know how anim works in cpp
			//UAnimInstance* animBp = mSkeletalMesh->GetAnimInstance();
		}
	}
}

// Called every frame
void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (mCharacter)
	{
		if (mCharacter->GetSightCameraEnabled() && !currentProjectile)
		{
			UpdateIKArm();
		}
	}
	if (currentProjectile && currentProjectile->IsColliding())
	{
		if (mCharacter)
		{

			mDirection = mDestination - mCharacter->GetActorLocation();
			float dist = mDirection.Size();

			mDirection.Normalize();

			if (dist > offsetStop)
			{
				mCharacter->GetCharacterMovement()->GroundFriction = 0.0f;
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
						mCharacter->ResetFriction();

						mSkeletalMesh->UnHideBone(mIdBone);
						mSkeletalMesh->bRequiredBonesUpToDate = false;

						currentProjectile->Destroy();
						currentProjectile = nullptr;
					}
				}

			}
		}
	}
}
