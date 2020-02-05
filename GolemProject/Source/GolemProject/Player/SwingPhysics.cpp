// Fill out your copyright notice in the Description page of Project Settings.


#include "SwingPhysics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "Engine/World.h"

SwingPhysics::SwingPhysics()
{
}

SwingPhysics::~SwingPhysics()
{
}

SwingPhysics::SwingPhysics(class ACharacter*& _character, class AActor*& _hook)
{
	UWorld* world = _character->GetWorld();
	character = _character;
	target = _hook;

	if (world)
	{
		constraintActor = world->SpawnActor<APhysicsConstraintActor>();
	}
	
	if (character)
	{
		characterMovement = character->GetCharacterMovement();

		if (characterMovement)
			characterMovement->GravityScale = 0.0f;

		if (target)
		{
			lastPosition = character->GetActorLocation();
			length = FVector::Dist(character->GetActorLocation(), target->GetActorLocation());
		}
	}
}

void SwingPhysics::Tick(float _deltaTime)
{
}
