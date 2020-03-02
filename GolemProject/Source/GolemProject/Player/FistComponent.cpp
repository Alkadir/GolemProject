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
		if (IsTargetingFist && mCharacter->GetSightCameraEnabled() && CanFire)
		{
			UpdateIKArm();
			FVector end = mCamera->GetComponentLocation() + mCamera->GetForwardVector() * accuracy;
			FVector direction = end - GetHandPosition();
			FVector location = GetHandPosition();
			FVector scale;
			FRotator rotation = direction.Rotation();
			for (int i = 0; i < NumberBounce; ++i)
			{
				if (HelperAiming.Num() <= i)
				{
					HelperAiming.Add(world->SpawnActor<AActor>(HelperAimingClass));
				}
				if (HelperAiming[i] != nullptr)
				{
					HelperAiming[i]->SetActorLocation(location);
					FHitResult hitResult;
					HelperAiming[i]->SetActorRotation(rotation);
					scale = HelperAiming[i]->GetActorScale3D();
					FVector distance = direction * accuracy;
					scale.Z = distance.Size();
					HelperAiming[i]->SetActorScale3D(scale);
					if (world->LineTraceSingleByChannel(hitResult, location, end, ECollisionChannel::ECC_Visibility))
					{
						distance = hitResult.ImpactPoint - location;
						scale.Z = distance.Size() / 100.0f;
						HelperAiming[i]->SetActorScale3D(scale);
						if (hitResult.GetComponent()->ComponentHasTag("Bounce"))
						{
							direction = direction.MirrorByVector(hitResult.ImpactNormal);
							end = direction * accuracy;
							location = hitResult.ImpactPoint;
							rotation = direction.Rotation();
						}
						else
						{
							if (HelperAiming.Num() != 0)
							{
								for (int j = i + 1; j < HelperAiming.Num(); ++j)
								{
									HelperAiming[j]->Destroy();
									HelperAiming.RemoveAt(j);
								}
							}
							i = NumberBounce;
						}
					}
				}
			}
		}
		else
		{
			if (HelperAiming.Num() != 0)
			{
				for (int i = 0; i < HelperAiming.Num(); ++i)
				{
					if (HelperAiming[i] != nullptr)
						HelperAiming[i]->Destroy();
				}
				HelperAiming.Empty();
			}
		}
	}
}

