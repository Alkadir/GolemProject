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
	class UGrappleComponent* grapple;
	class UCableComponent* cable;
	class UWorld* world;

	FVector velocity;
	FVector lastLocation;
	FVector newLocation;
	FVector segment;
	FVector direction;

	float friction;
	float length;
	float dist;
	float diff;
	float percent;
	
	//values to edit
	float scaleGravity;
	float forceMovement;
	float speedRotation;

	bool isAlreadyConnected;
public:
	SwingPhysics();
	~SwingPhysics();
	SwingPhysics(class UGrappleComponent* _grappleHook);

	FORCEINLINE void SetTarget(AActor*& _target) { target = _target; };
	FORCEINLINE AActor*& GetTarget() { return target; };
	void AddForceMovement(FVector _direction);
	void Tick(float _deltaTime);
};
