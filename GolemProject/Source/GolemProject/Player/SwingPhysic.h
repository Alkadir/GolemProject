// Golem Project - Créajeux 2020

#pragma once

class GOLEMPROJECT_API USwingPhysic 
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

	
	float length;
	float minLength = 100.0f;
	float maxLength = 1000.0f;
	float dist;
	float diff;
	float percent;

	float scaleGravity = 4.0f;
	float friction = 0.9998f;
	float forceMovement = 5.0f;
	float speedRotation = 0.01f;

	bool isAlreadyConnected;
public:
	USwingPhysic(class UGrappleComponent* _grappleHook);
	USwingPhysic();
	~USwingPhysic();

	void SetTarget(AActor*& _target) { target = _target; };
	AActor*& GetTarget() { return target; };
	FVector GetVelocity() { return velocity; };
	FVector GetDirection() { return direction; };
	void AddForceMovement(FVector _direction);
	void Tick(float _deltaTime);

	void SetScaleGravity(float _scaleGravity) { scaleGravity = _scaleGravity; };
	void SetFriction(float _friction) { friction = _friction; };
	void SetForceMovement(float _forceMovement) { forceMovement = _forceMovement; };
	void SetSpeedRotation(float _speedRotation) { speedRotation = _speedRotation; };
	void SetMinLength(float _minLength) { minLength = _minLength; };
	void SetMaxLength(float _maxLength) { maxLength = _maxLength; };
};
