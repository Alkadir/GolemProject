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
	class AGolemProjectCharacter* character;
	class UCharacterMovementComponent* characterMovement;
	class AActor* target;
	
	class UCableComponent* cable;
public:
	SwingPhysics();
	~SwingPhysics();
	SwingPhysics(class UGrappleComponent* _grappleHook);

	FORCEINLINE void SetTarget(AActor*& _target) { target = _target; };
	FORCEINLINE AActor*& GetTarget() { return target; };

	void Tick(float _deltaTime);
};
