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

USwingPhysic::USwingPhysic()
{
}

USwingPhysic::~USwingPhysic()
{
	if (characterMovement)
	{
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
}

void USwingPhysic::AddForceMovement(FVector _direction)
{
	if (world)
	{
		direction = _direction;
		float dt = world->GetDeltaSeconds();
		if (dt > 1.0f / frameRateMin)
		{
			dt = 1.0f / frameRateMin;
		}
		movement = _direction * forceMovement * dt;
	}
}

void USwingPhysic::InvertVelocity(FVector _normal)
{
	float coef = 0.0f;
	FHitResult hit;

	velocity = -(1.0f + coef) * FVector::DotProduct(velocity, _normal) * _normal;
	character->SetActorLocation(character->GetActorLocation() + velocity, true, nullptr, ETeleportType::None);
}

void USwingPhysic::Tick(float _deltaTime)
{
	if (_deltaTime > 1.0f / frameRateMin)
	{
		_deltaTime = 1.0f / frameRateMin;
	}
	if (world && character && target && grapple && grapple->GetProjectile())
	{
		//transistion with the ue4 velocity 
		FVector tmpVel = (character->GetActorLocation() - lastLocation);
		if (tmpVel != FVector::ZeroVector)
		{
			velocity = tmpVel + movement;
			movement = FVector::ZeroVector;
		}

		newLocation = character->GetActorLocation()
			+ velocity * friction
			+ FVector(0.0f, 0.0f, world->GetGravityZ()) * _deltaTime * _deltaTime * scaleGravity;


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
	}
}

void USwingPhysic::ReduceRope()
{
	if (world)
	{
		if (length > minLength)
		{
			float dt = world->GetDeltaSeconds();
			if (dt > 1.0f / frameRateMin)
			{
				dt = 1.0f / frameRateMin;
			}
			length -= reduceRopeSpeed * dt;
		}
		else if (!bWasStopped)
		{
			bWasStopped = true;
			character->EndReducingRopeEvent();
			character->StopReducingRopeEvent();
		}
	}
}

void USwingPhysic::SetCameraDirection(FVector _direction)
{
	cameraDirection = _direction;
	bIsFistActive = true;
}
