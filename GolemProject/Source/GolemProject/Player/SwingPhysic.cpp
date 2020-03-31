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

USwingPhysic::USwingPhysic()
{
}

USwingPhysic::~USwingPhysic()
{
	if (characterMovement)
	{
		characterMovement->Activate();
		//not the best maybe it's a to do
		characterMovement->Velocity = (character->GetActorLocation() - lastLocation) / world->GetDeltaSeconds() * releaseForce;
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
		length = (lastLocation - target->GetActorLocation()).Size();
	}
	if (world && characterMovement)
	{
		velocity = characterMovement->Velocity * world->GetDeltaSeconds();
		direction = character->GetActorForwardVector()* characterMovement->Velocity.Size();
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
		movement = _direction * forceMovement * world->GetDeltaSeconds();
		velocity += movement;
	}
}

void USwingPhysic::InvertVelocity(FVector _normal)
{
	velocity = (velocity * 0.5f)*_normal;
}

void USwingPhysic::Tick(float _deltaTime)
{
	if (world && character && target)
	{
		newLocation = character->GetActorLocation()
			+ velocity * friction
			+ FVector(0.0f, 0.0f, world->GetGravityZ()) * FMath::Pow(_deltaTime, 2.0f) * scaleGravity;

		//transistion with the ue4 velocity 
		FVector tmpVel = (character->GetActorLocation() - lastLocation);
		if (tmpVel != FVector::ZeroVector)
		{
			velocity = tmpVel;
		}

		lastLocation = character->GetActorLocation();

		segment = newLocation - target->GetActorLocation();

		dist = segment.Size();
		diff = FMath::Clamp(length, minLength, maxLength) - dist;

		percent = (diff / dist) * 0.5f;

		newLocation += segment * percent;

		character->SetActorLocation(newLocation, true, nullptr, ETeleportType::None);

		//smooth rotationnal movement 
		FVector handtoCharacter = grapple->GetProjectile()->GetLocation() - character->GetVirtualRightHandPosition();

		FVector right = FVector::CrossProduct(FVector::DownVector, handtoCharacter - (direction * (velocity/world->GetDeltaSeconds()).Size()));
		handtoCharacter = FVector::CrossProduct(right, handtoCharacter);

		FRotator finalRotation = handtoCharacter.Rotation();
		FRotator rotation = FMath::Lerp(character->GetActorRotation(), finalRotation, speedRotation);
		character->SetActorRotation(rotation);
	}
}

