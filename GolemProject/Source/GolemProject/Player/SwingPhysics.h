// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class GOLEMPROJECT_API SwingPhysics
{
private:
	class ACharacter* character;
	class UCharacterMovementComponent* characterMovement;
	class AActor* target;

	FVector lastPosition;
	FVector newPosition;
	FVector velocity;
	float length;
	float friction;

public:
	SwingPhysics(class ACharacter*& _character, class AActor*& _hook);
	SwingPhysics();
	~SwingPhysics();

	FORCEINLINE void SetTarget(AActor*& _target) { target = _target; };
	FORCEINLINE AActor*& GetTarget() { return target; };

	void Tick(float _deltaTime);
};
