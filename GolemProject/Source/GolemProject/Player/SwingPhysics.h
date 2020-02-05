// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/*"*/
/**
 *
 */
class GOLEMPROJECT_API SwingPhysics
{
private:
	class APhysicsConstraintActor* constraintActor;
	class UPhysicsConstraintComponent* constraintComponent;
	class AGolemProjectCharacter* character;
	class UCharacterMovementComponent* characterMovement;
	class AActor* target;

	/*FVector lastPosition;
	FVector newPosition;
	FVector velocity;
	float length;
	float friction;*/

public:
	SwingPhysics();
	~SwingPhysics();
	SwingPhysics(class UGrappleComponent* _grappleHook);

	FORCEINLINE void SetTarget(AActor*& _target) { target = _target; };
	FORCEINLINE AActor*& GetTarget() { return target; };

	void Tick(float _deltaTime);
};
