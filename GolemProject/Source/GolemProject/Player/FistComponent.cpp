// Fill out your copyright notice in the Description page of Project Settings.


#include "FistComponent.h"
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
#include "Classes/Components/StaticMeshComponent.h"
#include "Interfaces/Targetable.h"
#include "GolemProjectGameMode.h"
#include "Player/FistProjectile.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UFistComponent::UFistComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFistComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* owner = GetOwner();
	mCharacter = Cast<AGolemProjectCharacter>(owner);
	mSkeletalMesh = mCharacter->GetMesh();
	mIdBone = mSkeletalMesh->GetBoneIndex("hand_l");
	UChildActorComponent* child = HelperLibrary::GetComponentByName<UChildActorComponent>(mCharacter, "ShoulderCamera");
	mCamera = HelperLibrary::GetComponentByName<UCameraComponent>(child->GetChildActor(), "Camera");
	world = GetWorld();
	CanFire = true;
}

void UFistComponent::UpdateIKArm()
{
	if (world && mCamera)
	{
		FVector offset = mCamera->GetForwardVector() * accuracy;
		mDirection = offset - mCharacter->GetActorLocation();
		IKposition = offset;
		mDirection.Z = 0.0f;
		mCharacter->SetActorRotation(mDirection.Rotation());

		//I don't know how anim works in cpp
		//UAnimInstance* animBp = mSkeletalMesh->GetAnimInstance();

	}
}

FVector UFistComponent::GetHandPosition()
{
	FVector pos = FVector::ZeroVector;
	if (mSkeletalMesh)
	{
		pos = mSkeletalMesh->GetBoneTransform(mIdBone).GetLocation();
	}
	return pos;
}

void UFistComponent::SetIKArm(FVector& _lookAt, bool& _isBlend)
{
	if (!currentProjectile)
		_lookAt = IKposition;
	_isBlend = (mCharacter->GetSightCameraEnabled());
}

void UFistComponent::GoToDestination()
{
	if (!currentProjectile && CanFire)
	{
		if (world && mCamera)
		{
			mSkeletalMesh->HideBone(mIdBone, EPhysBodyOp::PBO_None);

			currentProjectile = world->SpawnActor<AFistProjectile>(fistProjectileClass, mSkeletalMesh->GetBoneTransform(mIdBone));
			if (currentProjectile)
			{
				FVector offset = mCamera->GetComponentLocation() + mCamera->GetForwardVector() * accuracy;
				FVector direction = (offset - currentProjectile->GetActorLocation());
				direction /= direction.Size();

				currentProjectile->Instigator = mCharacter->GetInstigator();
				currentProjectile->SetOwner(mCharacter);
				currentProjectile->LaunchFist(direction, true);
				currentProjectile = nullptr;
				world->GetTimerManager().SetTimer(TimerHandleFire, this, &UFistComponent::ResetFire, TimerFire, false);
				CanFire = false;
			}
		}
	}
}

void UFistComponent::DisplayTrajectory()
{

}

void UFistComponent::ResetFire()
{
	CanFire = true;
	mSkeletalMesh->UnHideBone(mIdBone);
	mSkeletalMesh->bRequiredBonesUpToDate = false;
}

// Called every frame
void UFistComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (mCharacter)
	{
		if (IsTargetingFist && mCharacter->GetSightCameraEnabled() && !currentProjectile)
		{
			UpdateIKArm();
			FHitResult hitResult;
			FVector end = mCamera->GetComponentLocation() + mCamera->GetForwardVector() * accuracy;
			DrawDebugLine(world, GetHandPosition(), end, FColor::Emerald, false, 0.0f, 0, 2.0f);
			FVector dir = end - GetHandPosition();
			
			if (world->LineTraceSingleByChannel(hitResult, GetHandPosition(), end, ECollisionChannel::ECC_Visibility))
			{
				if (hitResult.GetComponent()->ComponentHasTag("Bounce"))
				{
					FVector direction;
					direction = dir.MirrorByVector(hitResult.ImpactNormal);
					DrawDebugLine(world, hitResult.ImpactPoint, direction * accuracy, FColor::Emerald, false, 0.0f, 0, 2.0f);
				}
			}
		}
	}
}

