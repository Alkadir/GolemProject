// Golem Project - Créajeux 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SwingPhysic.generated.h"

/**
 *
 */
UCLASS()
class GOLEMPROJECT_API USwingPhysic : public UObject
{
	GENERATED_BODY()
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

	float length;
	float dist;
	float diff;
	float percent;

	float scaleGravity = 4.0f;
	float friction = 0.9998f;
	float forceMovement = 5.0f;
	float speedRotation = 0.01f;

	bool isAlreadyConnected;
public:
	USwingPhysic();

	void Initialize(class UGrappleComponent* _grappleHook);
	void Destroy();
	FORCEINLINE void SetTarget(AActor*& _target) { target = _target; };
	FORCEINLINE AActor*& GetTarget() { return target; };
	FORCEINLINE FVector GetVelocity() { return velocity; };
	FORCEINLINE FVector GetDirection() { return direction; };
	void AddForceMovement(FVector _direction);
	void Tick(float _deltaTime);

	FORCEINLINE void SetScaleGravity(float _scaleGravity) { scaleGravity = _scaleGravity; };
	FORCEINLINE void SetFriction(float _friction) { friction = _friction; };
	FORCEINLINE void SetForceMovement(float _forceMovement) { forceMovement = _forceMovement; };
	FORCEINLINE void SetSpeedRotation(float _speedRotation) { speedRotation = _speedRotation; };
};
