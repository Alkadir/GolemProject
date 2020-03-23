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
#include "Helpers/HelperLibrary.h"

USwingPhysic::USwingPhysic()
{
}

USwingPhysic::~USwingPhysic()
{
	if (characterMovement)
	{
		float forcePower = 100.0f;
		characterMovement->Activate();
		//not the best maybe it's a to do
		characterMovement->Velocity = (character->GetActorLocation() - lastLocation) * forcePower;
	}
}

USwingPhysic::USwingPhysic(UGrappleComponent* _grappleHook)
{
	world = _grappleHook->GetWorld();
	grapple = _grappleHook;
	character = _grappleHook->GetCharacter();
	characterMovement = character->GetCharacterMovement();
	target = _grappleHook->GetClosestGrapplingHook();
	lastLocation = character->GetActorLocation();
	isAlreadyConnected = false;

	length = FVector::Distance(target->GetActorLocation(), character->GetActorLocation());

	velocity = characterMovement->Velocity * world->GetDeltaSeconds();
	characterMovement->Deactivate();
}

void USwingPhysic::AddForceMovement(FVector _direction)
{
	if (world)
	{
		direction = _direction;
		velocity += _direction * forceMovement * world->GetDeltaSeconds();
	}
}

void USwingPhysic::Tick(float _deltaTime)
{
	if (world && character && target)
	{
		newLocation = character->GetActorLocation()
			+ velocity * friction
			+ FVector(0.0f, 0.0f, world->GetGravityZ()) * FMath::Pow(_deltaTime, 2.0f) * scaleGravity;
		velocity = (character->GetActorLocation() - lastLocation);
		lastLocation = character->GetActorLocation();

		segment = newLocation - target->GetActorLocation();

		dist = segment.Size();
		diff = length - dist;

		percent = (diff / dist) * 0.5f;

		newLocation += segment * percent;

		character->SetActorLocation(newLocation, true, nullptr, ETeleportType::None);

		//smooth rotationnal movement 
		direction.Z = velocity.Z * 0.5f;
		FRotator rot = FMath::Lerp(character->GetActorRotation(), direction.Rotation(), speedRotation);
		character->SetActorRotation(rot);
		isAlreadyConnected = true;
	}
}

