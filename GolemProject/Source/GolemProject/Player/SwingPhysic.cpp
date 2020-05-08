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
		characterMovement->Velocity = (character->GetActorLocation() - lastLocation) / world->GetDeltaSeconds() * releaseForce;
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
		velocity = characterMovement->Velocity * world->GetDeltaSeconds();
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
		movement = _direction * forceMovement * world->GetDeltaSeconds();
	}
}

void USwingPhysic::InvertVelocity()
{
	FHitResult hit;
	FVector unpackedVelocity = (velocity / world->GetDeltaSeconds());
	if (world && world->LineTraceSingleByChannel(hit, character->GetActorLocation(), character->GetActorLocation() + unpackedVelocity * 5.0f, ECollisionChannel::ECC_Visibility))
	{
		float size = velocity.Size();
		velocity = FMath::Max(5.0f, size) * hit.Normal;
	}
}

void USwingPhysic::Tick(float _deltaTime)
{
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
			+ FVector(0.0f, 0.0f, world->GetGravityZ()) * FMath::Pow(_deltaTime, 2.0f) * scaleGravity;


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
			length -= reduceRopeSpeed * world->GetDeltaSeconds();
	}
}

void USwingPhysic::SetCameraDirection(FVector _direction)
{
	cameraDirection = _direction;
	bIsFistActive = true;
}
