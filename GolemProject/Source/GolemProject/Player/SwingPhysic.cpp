// Golem Project - Créajeux 2020
#include "SwingPhysic.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GolemProjectCharacter.h"
#include "GameFramework/Actor.h"
#include "Classes/Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "Player/GrappleComponent.h"
#include "Player/ProjectileHand.h"
#include "Helpers/HelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsPublic.h"

USwingPhysic::USwingPhysic()
{
}

USwingPhysic::~USwingPhysic()
{
	if (characterMovement)
	{
		if (world)
		{
			FPhysScene* PScene = world->GetPhysicsScene();
			if (PScene)
			{
				// Unregister substep delegate when component is removed
				PScene->OnPhysSceneStep.Remove(OnPhysSceneStepHandle);
			}
		}

		characterMovement->Activate();
		//not the best maybe it's a to do
		if (world)
		{
			float dt = world->GetDeltaSeconds();
			if (dt > 1.0f / frameRateMin)
			{
				dt = 1.0f / frameRateMin;
			}
			characterMovement->Velocity = (character->GetActorLocation() - lastLocation) / dt * releaseForce;
		}
		target = nullptr;
		grapple = nullptr;
		character = nullptr;
	}
}

USwingPhysic::USwingPhysic(UGrappleComponent* _grappleHook)
{
	if (_grappleHook == nullptr)
	{
		return;
	}
	world = _grappleHook->GetWorld();
	grapple = _grappleHook;
	character = _grappleHook->GetCharacter();

	velocity = FVector::ZeroVector;
	lastLocation = FVector::ZeroVector;
	lastVelocity = FVector::ZeroVector;
	newLocation = FVector::ZeroVector;
	segment = FVector::ZeroVector;
	direction = FVector::ZeroVector;
	movement = FVector::ZeroVector;
	cameraDirection = FVector::ZeroVector;


	if (character)
	{
		characterMovement = character->GetCharacterMovement();
		lastLocation = character->GetActorLocation();
	}

	target = _grappleHook->GetClosestGrapplingHook();

	if (target)
	{
		length = (lastLocation - grapple->GetProjectile()->GetLocation()).Size();
	}
	if (world && characterMovement)
	{
		float dt = world->GetDeltaSeconds();
		if (dt > 1.0f / frameRateMin)
		{
			dt = 1.0f / frameRateMin;
		}
		velocity = characterMovement->Velocity * dt;
		direction = character->GetActorForwardVector() * 1000.0f;
	}
	if (characterMovement)
	{
		characterMovement->Deactivate();
	}

	if (world)
	{
		FPhysScene* PScene = world->GetPhysicsScene();
		if (PScene)
		{
			// Register UE 4.15+ substep delegate
			OnPhysSceneStepHandle = PScene->OnPhysSceneStep.AddRaw(this, &USwingPhysic::PhysSceneStep);
		}
	}
}

void USwingPhysic::AddForceMovement(FVector _direction)
{
	if (world)
	{
		direction = _direction;
	}
}

void USwingPhysic::InvertVelocity(FVector _normal)
{
	float coef = 0.0f;
	FHitResult hit;

	velocity = -(1.0f + coef) * FVector::DotProduct(velocity, _normal) * _normal;
	character->SetActorLocation(character->GetActorLocation() + velocity, true, nullptr, ETeleportType::None);
}

void USwingPhysic::ReduceRope()
{
	if (world)
	{
		bIsReducingRope = true;
	}
}

void USwingPhysic::SetCameraDirection(FVector _direction)
{
	cameraDirection = _direction;
	bIsFistActive = true;
}

void USwingPhysic::PhysSceneStep(FPhysScene_PhysX* _PhysScene, float _dt)
{
	if (!world)
		return;

	float dt = _dt;
	if (world && character && target && grapple && grapple->GetProjectile())
	{
		if (bIsReducingRope)
		{
			if (length > minLength)
			{
				length -= reduceRopeSpeed * dt;
			}
			else if (!bWasStopped)
			{
				bWasStopped = true;
				character->EndReducingRopeEvent();
				character->StopReducingRopeEvent();
			}
		}

		if (bIsMoving)
		{
			movement = direction * forceMovement * dt;
		}

		//transistion with the ue4 velocity
		FVector tmpVel = (character->GetActorLocation() - lastLocation);
		if (tmpVel != FVector::ZeroVector)
		{
			velocity = tmpVel + movement;
			movement = FVector::ZeroVector;
		}

		acceleration = FVector(0.0f, 0.0f, world->GetGravityZ()) * dt * dt * scaleGravity;
		newLocation = character->GetActorLocation()
			+ velocity * friction
			+ acceleration;

		lastLocation = character->GetActorLocation();

		segment = newLocation - grapple->GetProjectile()->GetLocation();

		dist = segment.Size();
		diff = FMath::Clamp(length, minLength, maxLength) - dist;

		percent = (diff / dist) * 0.5f;

		newLocation += segment * percent;

		character->SetActorLocation(newLocation, true, nullptr, ETeleportType::None);

		//smooth rotationnal movement 
		FVector handtoCharacter = grapple->GetProjectile()->GetLocation() - character->GetVirtualRightHandPosition();

		FRotator finalRotation;

		if (!bIsFistActive)
			finalRotation = UKismetMathLibrary::MakeRotFromZX(handtoCharacter, tmpVel);
		else
			finalRotation = UKismetMathLibrary::MakeRotFromZX(handtoCharacter, cameraDirection);

		FRotator rotation = FMath::Lerp(character->GetActorRotation(), finalRotation, speedRotation);
		character->SetActorRotation(rotation);

		bIsFistActive = false;
		bIsMoving = false;
		bIsReducingRope = false;
	}
}