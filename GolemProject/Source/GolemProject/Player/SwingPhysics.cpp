// Fill out your copyright notice in the Description page of Project Settings.


#include "SwingPhysics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Actor.h"

SwingPhysics::SwingPhysics(class ACharacter*& _character, class AActor*& _hook)
{
	character = _character;
	target = _hook;

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

SwingPhysics::SwingPhysics()
{
}

SwingPhysics::~SwingPhysics()
{
}

void SwingPhysics::Tick(float _deltaTime)
{
	float dist, diff, percent;

	lastPosition = character->GetActorLocation();

	newPosition = character->GetActorLocation()
		+ velocity * friction
		+ characterMovement->GetGravityZ() * _deltaTime*_deltaTime;

	FVector segment = newPosition - target->GetActorLocation();

	dist = segment.Size();
	diff = length - dist;

	percent = (diff / dist) / 2.0f;

	newPosition += segment * percent;

	character->SetActorLocation(newPosition);
}
