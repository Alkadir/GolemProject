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
#include "Classes/Components/StaticMeshComponent.h"
#include "Interfaces/Targetable.h"
#include "GolemProjectGameMode.h"

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
	if (UWorld* world = GetWorld())
	{
		GameMode = Cast<AGolemProjectGameMode>(world->GetAuthGameMode());
	}
	if (APlayerController* ctrl = Cast<APlayerController>(mCharacter->GetController()))
	{
		PlayerCameraManager = ctrl->PlayerCameraManager;
	}
}

void UGrappleComponent::CheckElementTargetable()
{
	if (mCharacter == nullptr) return;
	if (GameMode == nullptr) return;
	TArray<AActor*> allActors = GameMode->GetActorsTargetable();
	if (allActors.Num() <= 0) return;
	TArray<AActor*> actorCloseEnough;

	if (UCameraComponent* followingCam = mCharacter->GetFollowCamera())
	{
		if (UWorld* world = GetWorld())
		{
			for (AActor* actor : allActors)
			{
				if (!actor->Implements<UTargetable>()) continue;

				if (FVector::DistSquared(actor->GetActorLocation(), mCharacter->GetActorLocation()) < maxDistance * maxDistance &&
					FVector::DistSquared(actor->GetActorLocation(), mCharacter->GetActorLocation()) > minDistance * minDistance)
				{
					actorCloseEnough.Add(actor);
				}
			}
			HelperLibrary::SortActorsByDistanceTo(actorCloseEnough, mCharacter);
			for (AActor* actor : actorCloseEnough)
			{
				// > 0 object seen
				FVector FromSoftware = (actor->GetActorLocation() - mCharacter->GetActorLocation()).GetSafeNormal();
				float dot = FVector::DotProduct(followingCam->GetForwardVector(), FromSoftware);
				//to change and finish
				if (dot > minDot&& dot < maxDot)
				{
					FHitResult hitResult;
					if (world->LineTraceSingleByChannel(hitResult, mCharacter->GetActorLocation(), actor->GetActorLocation(), ECollisionChannel::ECC_Visibility))
					{
						if (ClosestGrapplingHook != nullptr && ClosestGrapplingHook == hitResult.GetActor()) return;
						ITargetable* target = Cast<ITargetable>(hitResult.GetActor());
						if (target != nullptr)
						{
							if (ITargetable* Lasttarget = Cast<ITargetable>(ClosestGrapplingHook))
							{
								Lasttarget->Execute_DestroyHUD(ClosestGrapplingHook);
							}
							target->Execute_CreateHUD(hitResult.GetActor());
							ClosestGrapplingHook = actor;
							return;
						}
					}
				}
			}
			if (ClosestGrapplingHook != nullptr)
			{
				if (ITargetable* Lasttarget = Cast<ITargetable>(ClosestGrapplingHook))
				{
					Lasttarget->Execute_DestroyHUD(ClosestGrapplingHook);
				}
				ClosestGrapplingHook = nullptr;
			}
		}
	}
}

//launch projectile
void UGrappleComponent::GoToDestination(bool _isAssisted)
{
	if (_isAssisted && ClosestGrapplingHook == nullptr) return;
	if (!currentProjectile)
	{
		UWorld* world = GetWorld();

		if (world && mCamera)
		{
			mSkeletalMesh->HideBone(mIdBone, EPhysBodyOp::PBO_None);

			currentProjectile = world->SpawnActor<AProjectileHand>(handProjectileClass, mSkeletalMesh->GetBoneTransform(mIdBone));
			if (currentProjectile)
			{
				FVector offset = _isAssisted ? ClosestGrapplingHook->GetActorLocation() : mCamera->GetForwardVector() * accuracy;
				FVector direction = (offset - currentProjectile->GetActorLocation()).GetSafeNormal();

				currentProjectile->Instigator = mCharacter->GetInstigator();
				currentProjectile->SetOwner(mCharacter);
				currentProjectile->LaunchProjectile(direction, this);
			}
		}
	}
}

//cancel projectile
void UGrappleComponent::Cancel()
{
	if (currentProjectile && !currentProjectile->IsColliding())
	{
		currentProjectile->SetComingBack(true);
	}
}

void UGrappleComponent::SetIKArm(FVector& _lookAt, bool& _isBlend)
{
	_lookAt = IKposition;
	_isBlend = (mCharacter->GetSightCameraEnabled() || currentProjectile);
}

FVector UGrappleComponent::GetHandPosition()
{
	FVector pos = FVector::ZeroVector;
	if (mSkeletalMesh)
	{
		pos = mSkeletalMesh->GetBoneTransform(mIdBone).GetLocation();
	}
	return pos;
}

void UGrappleComponent::UpdateIKArm()
{
	UWorld* world = GetWorld();

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


// Called every frame
void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckElementTargetable();
	if (mCharacter)
	{
		if (mCharacter->GetSightCameraEnabled() && !currentProjectile)
		{
			UpdateIKArm();
		}
	}

	if (currentProjectile)
	{
		mDirection = currentProjectile->GetMeshComponent()->GetComponentLocation() - mSkeletalMesh->GetBoneTransform(mIdBone).GetLocation();
		float dist = mDirection.Size();

		if (currentProjectile->IsComingBack())
		{
			if (dist < offsetStop)
			{
				PlayerIsNear();
				return;
			}
		}
		else if (currentProjectile->IsColliding())
		{
			if (mCharacter)
			{
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
					PlayerIsNear();
					return;
				}
			}
		}
		else
		{
			if (dist > maxDistance)
			{
				currentProjectile->SetComingBack(true);
			}
		}
	}
}

void UGrappleComponent::PlayerIsNear()
{
	//Find destination stop player

	if (AController* ctrl = mCharacter->GetController())
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

