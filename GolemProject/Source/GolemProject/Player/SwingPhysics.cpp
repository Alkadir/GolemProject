// Fill out your copyright notice in the Description page of Project Settings.


#include "SwingPhysics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GolemProjectCharacter.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Classes/Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "Player/GrappleComponent.h"
#include "Helpers/HelperLibrary.h"

SwingPhysics::SwingPhysics()
{
}

SwingPhysics::~SwingPhysics()
{
}

SwingPhysics::SwingPhysics(UGrappleComponent* _grappleHook)
{
	UWorld* world = _grappleHook->GetWorld();
	character = _grappleHook->GetCharacter();
	characterMovement = character->GetCharacterMovement();
	target = _grappleHook->GetClosestGrapplingHook();
	FVector vec = (target->GetActorLocation() - character->GetActorLocation());
	
	float dist = vec.Size();
	vec /= dist;
	dist *= 0.5f;

	if (world)
	{
		FVector constraintLocation = character->GetActorLocation() + dist * vec;
		constraintActor = world->SpawnActor<APhysicsConstraintActor>(constraintLocation, FRotator::ZeroRotator);
		HelperLibrary::Print(target->GetFName().ToString());
		constraintComponent = constraintActor->GetConstraintComp();
		constraintComponent->ConstraintActor1 = character->GetMesh()->GetAttachmentRootActor();
		constraintComponent->ConstraintActor2 = target;
	}

	/*if (character)
	{*/


	/*if (characterMovement)
		characterMovement->GravityScale = 0.0f;*/

		/*if (target)
		{
			lastPosition = character->GetActorLocation();
			length = FVector::Dist(character->GetActorLocation(), target->GetActorLocation());
		}*/
		//}
}

void SwingPhysics::Tick(float _deltaTime)
{
}
