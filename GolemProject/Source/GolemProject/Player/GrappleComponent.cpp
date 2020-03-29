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
#include "SwingPhysic.h"
#include "DashComponent.h"
#include "DrawDebugHelpers.h"
#include "Player/Rope.h"
#include "Components/CapsuleComponent.h"

//#include "DrawDebugHelpers.h"

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
	SetTickGroup(ETickingGroup::TG_PrePhysics);
	AActor* owner = GetOwner();
	mCharacter = Cast<AGolemProjectCharacter>(owner);
	if (mCharacter)
	{
		mSkeletalMesh = mCharacter->GetMesh();
		if (mSkeletalMesh)
		{
			mIdBone = mSkeletalMesh->GetBoneIndex("hand_r");
		}
	}
	if (UChildActorComponent* child = HelperLibrary::GetComponentByName<UChildActorComponent>(mCharacter, "ShoulderCamera"))
	{
		mCamera = HelperLibrary::GetComponentByName<UCameraComponent>(child->GetChildActor(), "Camera");
	}
	IsFiring = false;
	world = GetWorld();

	if (world)
	{
		GameMode = Cast<AGolemProjectGameMode>(world->GetAuthGameMode());
	}
	if (mCharacter)
	{
		if (APlayerController* ctrl = Cast<APlayerController>(mCharacter->GetController()))
		{
			PlayerCameraManager = ctrl->PlayerCameraManager;
		}
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
		if (world)
		{
			for (AActor* actor : allActors)
			{
				if (actor == nullptr || !actor->Implements<UTargetable>()) continue;
				//get all the actors that are close to the player
				if (FVector::DistSquared(actor->GetActorLocation(), mCharacter->GetActorLocation()) < maxDistanceSwinging * maxDistanceSwinging &&
					FVector::DistSquared(actor->GetActorLocation(), mCharacter->GetActorLocation()) > minDistance* minDistance)
				{
					actorCloseEnough.Add(actor);
				}
			}
			HelperLibrary::SortActorsByDistanceTo(actorCloseEnough, mCharacter);
			if (followingCam)
			{
				for (AActor* actor : actorCloseEnough)
				{
					if (actor == nullptr) continue;
					// > 0 object seen
					FVector FromSoftware = (actor->GetActorLocation() - mCharacter->GetActorLocation());
					FromSoftware.Normalize();
					float dot = FVector::DotProduct(followingCam->GetForwardVector(), FromSoftware);
					//to change and finish
					if (dot > minDot&& dot < maxDot)
					{
						FHitResult hitResult;
						if (world->LineTraceSingleByChannel(hitResult, GetHandPosition(), actor->GetActorLocation(), ECollisionChannel::ECC_Visibility))
						{
							if (ClosestGrapplingHook != nullptr && ClosestGrapplingHook == hitResult.GetActor()) return;
							if (ITargetable* target = Cast<ITargetable>(hitResult.GetActor()))
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
	if (_isAssisted && ClosestGrapplingHook == nullptr || mSkeletalMesh == nullptr) return;
	if (!currentProjectile)
	{
		if (world && mCamera)
		{

			currentProjectile = world->SpawnActor<AProjectileHand>(handProjectileClass, mSkeletalMesh->GetBoneTransform(mIdBone));
			if (currentProjectile)
			{
				mSkeletalMesh->HideBone(mIdBone, EPhysBodyOp::PBO_None);
				FVector offset = _isAssisted ? ClosestGrapplingHook->GetActorLocation() : (mCamera->GetComponentLocation() + mCamera->GetForwardVector() * accuracy);
				FVector direction = (offset - currentProjectile->GetActorLocation());
				direction.Normalize();

				if (currentProjectile->GetMeshComponent())
				{
					mLastLocation = currentProjectile->GetLocation();
				}
				mDistance = 0.0f;
				//DrawDebugLine(world, mCamera->GetComponentLocation(), offset, FColor::Red, true);

				currentProjectile->Instigator = mCharacter->GetInstigator();
				currentProjectile->SetOwner(mCharacter);
				currentProjectile->LaunchProjectile(direction, this);
				IsFiring = true;
				bIsAssisted = _isAssisted;
				currentProjectile->SetAssisted(_isAssisted);

				//Create the rope visual 
				rope = world->SpawnActor<ARope>(ropeClass);
				rope->SetGrappleComponent(this);
			}
		}
	}
}

//cancel projectile
void UGrappleComponent::Cancel()
{
	if (currentProjectile && !currentProjectile->IsCollidingGrappling() && !currentProjectile->IsCollidingSwinging())
	{
		currentProjectile->SetComingBack(true);
	}
}

void UGrappleComponent::SetIKArm(FVector& _lookAt, bool& _isBlend)
{
	if (!currentProjectile)
		_lookAt = IKposition;
	else if (currentProjectile->GetMeshComponent())
		_lookAt = currentProjectile->GetLocation();
	else
		_lookAt = currentProjectile->GetActorLocation();

	if (mCharacter)
	{
		_isBlend = (!IsTargetingGrapple || mCharacter->GetSightCameraEnabled() || currentProjectile);
	}
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

FVector UGrappleComponent::GetVirtualRightHandPosition()
{
	FVector pos = FVector::ZeroVector;
	if (mSkeletalMesh)
	{
		int id = mSkeletalMesh->GetBoneIndex("VB hand_r");
		pos = mSkeletalMesh->GetBoneTransform(id).GetLocation();
	}
	return pos;
}

FVector UGrappleComponent::GetVirtualLeftHandPosition()
{
	FVector pos = FVector::ZeroVector;
	if (mSkeletalMesh)
	{
		int id = mSkeletalMesh->GetBoneIndex("VB hand_l");
		pos = mSkeletalMesh->GetBoneTransform(id).GetLocation();
	}
	return pos;
}

void UGrappleComponent::UpdateIKArm()
{
	if (world && mCamera && mCharacter)
	{
		FVector offset = mCamera->GetForwardVector() * accuracy;
		mDirection = offset - mCharacter->GetActorLocation();
		IKposition = offset;
		mDirection.Z = 0.0f;

		//Rotate character when he is aiming something
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

	//Update Ik Arm animation
	if (mCharacter && mCamera)
	{
		if (IsTargetingGrapple && mCharacter->GetSightCameraEnabled() && !currentProjectile)
		{
			UpdateIKArm();
			FVector end = mCamera->GetComponentLocation() + mCamera->GetForwardVector() * maxDistanceGrappling;
			FVector direction = end - GetHandPosition();
			FVector location = GetHandPosition();
			FVector scale;
			FRotator rotation = direction.Rotation();
			if (HelperAiming == nullptr)
			{
				HelperAiming = world->SpawnActor<AActor>(HelperAimingClass);
			}
			else
			{
				HelperAiming->SetActorLocation(location);
				FHitResult hitResult;
				HelperAiming->SetActorRotation(rotation);
				scale = HelperAiming->GetActorScale3D();
				FVector distance = direction.GetSafeNormal() * maxDistanceGrappling;
				scale.Z = distance.Size() / 100.0f;
				HelperAiming->SetActorScale3D(scale);
				if (world->LineTraceSingleByChannel(hitResult, location, end, ECollisionChannel::ECC_Visibility))
				{
					distance = hitResult.ImpactPoint - location;
					scale.Z = distance.Size() / 100.0f;
					HelperAiming->SetActorScale3D(scale);
				}
			}
		}
		else
		{
			if (HelperAiming != nullptr)
			{
				HelperAiming->Destroy();
				HelperAiming = nullptr;
			}
		}
	}
	if (currentProjectile && currentProjectile->GetMeshComponent() && mSkeletalMesh)
	{
		mDirection = currentProjectile->GetLocation() - GetVirtualRightHandPosition();
		mDistance += FVector::Dist(mLastLocation, currentProjectile->GetLocation());
		float distanceWithCharacter = mDirection.Size();
		mLastLocation = currentProjectile->GetLocation();
		if (currentProjectile->IsComingBack())
		{
			if (distanceWithCharacter < offsetStop)
			{
				PlayerIsNear();
				return;
			}
		}
		else if (currentProjectile->IsCollidingGrappling())
		{
			if (mCharacter)
			{
				if (!bIsAssisted)
				{
					if (distanceWithCharacter > offsetStop)
					{
						AttractCharacter();
					}
					else
					{
						PlayerIsNear();
						return;
					}
				}
			}
		}
		else if (currentProjectile->IsCollidingSwinging())
		{
			if (mCharacter && bIsAssisted)
			{
				//Create the swing physics for the player
				if (!swingPhysic && ClosestGrapplingHook)
				{
					swingPhysic = new USwingPhysic(this);

					swingPhysic->SetScaleGravity(scaleGravity);
					swingPhysic->SetFriction(friction);
					swingPhysic->SetForceMovement(forceMovement);
					swingPhysic->SetSpeedRotation(speedRotation);
					swingPhysic->SetMinLength(minDistanceSwinging);
					swingPhysic->SetMaxLength(maxDistanceSwinging);
					swingPhysic->SetReleaseForce(releaseForce);

					//Reset dash when the player grappled something
					if (UDashComponent* dashComp = mCharacter->FindComponentByClass<UDashComponent>())
						dashComp->ResetDashInAir();
				}
			}

			CheckGround();
		}
		else
		{
			if (mDistance > maxDistanceGrappling)
			{
				currentProjectile->SetComingBack(true);
			}
		}

		//if swing Physics exists we have to tick it
		if (swingPhysic)
			swingPhysic->Tick(DeltaTime);
	}
}

void UGrappleComponent::StopSwingPhysics()
{
	if (swingPhysic && currentProjectile)
	{
		bIsAssisted = false;
		delete swingPhysic;
		swingPhysic = nullptr;

		currentProjectile->SetComingBack(true);
	}
}

void UGrappleComponent::CheckGround()
{
	if (world)
	{
		if (mCharacter && mCharacter->GetCapsuleComponent())
		{
			float height = mCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - radiusOnGround * 0.5f;
			FVector location = mCharacter->GetActorLocation() - mCharacter->GetActorUpVector() * height;

			TArray<AActor*>OutActors;
			TArray<AActor*>ActorsToIgnore;
			TArray<TEnumAsByte<EObjectTypeQuery>>ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
			ActorsToIgnore.Add(mCharacter);
		
			if (UKismetSystemLibrary::SphereOverlapActors(world, location, radiusOnGround, ObjectTypes, NULL, ActorsToIgnore, OutActors))
			{
				if (swingPhysic)
					StopSwingPhysics();

				if (currentProjectile)
					currentProjectile->SetComingBack(true);
			}
		}
	}
}

void UGrappleComponent::PlayerIsNear()
{
	if (mCharacter)
	{
		//Find destination stop player

		if (mCharacter->GetCharacterMovement() && currentProjectile->IsCollidingGrappling())
		{
			mCharacter->GetCharacterMovement()->Velocity *= stopScaleVelocity;
		}
		mCharacter->ResetFriction();

		if (mSkeletalMesh)
		{
			mSkeletalMesh->UnHideBone(mIdBone);
			mSkeletalMesh->bRequiredBonesUpToDate = false;
		}

		if (rope)
			rope->Destroy();

		if (currentProjectile)
		{
			currentProjectile->Destroy();
		}
		rope = nullptr;
		currentProjectile = nullptr;
		IsFiring = false;
	}
}

void UGrappleComponent::AttractCharacter()
{
	FVector tempDir;
	mDirection.Normalize();
	tempDir = mDirection;
	if (mCharacter && mCharacter->GetCharacterMovement())
	{
		mCharacter->GetCharacterMovement()->GroundFriction = 0.0f;
		mCharacter->LaunchCharacter(mDirection * velocity, true, true);

		//change rotation player when the grapple isn't assisted
		FRotator finalRotation = tempDir.Rotation();
		finalRotation.Add(-90.0f, 0.0f, 0.0f);

		FRotator rotation = FMath::Lerp(mCharacter->GetActorRotation(), finalRotation, 0.1f);
		mCharacter->SetActorRotation(rotation);

		if (world)
		{
			//check if the player doesn't collide with anything(in front of him)
			FHitResult hit;
			if (world->LineTraceSingleByChannel(hit, mCharacter->GetActorLocation(), mCharacter->GetActorLocation() + mDirection * offsetBlockingObject, ECollisionChannel::ECC_Visibility))
			{
				if (mCharacter)
					mCharacter->GetCharacterMovement()->Velocity *= stopScaleVelocity;

				currentProjectile->SetComingBack(true);
			}
		}
	}
}
