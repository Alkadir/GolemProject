// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrappleComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GOLEMPROJECT_API UGrappleComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	class UWorld* world;
	class UCameraComponent* mCamera;
	class AGolemProjectCharacter* mCharacter;
	class USkeletalMeshComponent* mSkeletalMesh;
	class AProjectileHand* currentProjectile;
	class AActor* ClosestGrapplingHook;
	class AGolemProjectGameMode* GameMode;
	class APlayerCameraManager* PlayerCameraManager;
	class USwingPhysic* swingPhysic = nullptr;
	class ARope* rope = nullptr;
	class AActor* HelperAiming;

	FVector mDestination;
	FVector mDirection;
	FVector mLastLocation;

	int32 mIdBone;
	bool bIsAssisted = false;
	float accuracy = 100000.0f;

	//values to edit
	UPROPERTY(EditAnywhere, Category = "Swing Physics", meta = (AllowPrivateAccess = "true"))
	float minDistanceSwinging = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Swing physics", meta = (AllowPrivateAccess = "true"))
	float maxDistanceSwinging = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Swing Physics", meta = (AllowPrivateAccess = "true"))
	float scaleGravity = 4.0f;

	UPROPERTY(EditAnywhere, Category = "Swing Physics", meta = (AllowPrivateAccess = "true"))
	float friction = 0.9998f;

	UPROPERTY(EditAnywhere, Category = "Swing Physics", meta = (AllowPrivateAccess = "true"))
	float forceMovement = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Swing Physics", meta = (AllowPrivateAccess = "true"))
	float speedRotation = 0.06f;

	UPROPERTY(EditAnywhere, Category = "Swing Physics", meta = (AllowPrivateAccess = "true"))
	float releaseForce = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "Swing Physics", meta = (AllowPrivateAccess = "true"))
	float radiusOnGround = 25.0f;
	
	UPROPERTY(EditAnywhere, Category = "Swing Render", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARope> ropeClass;

	float mDistance;
	
protected:
	UPROPERTY(EditAnywhere, Category = "projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectileHand> handProjectileClass;

	UPROPERTY(EditAnywhere, Category = Help)
	TSubclassOf<class AActor> HelperAimingClass;

	UPROPERTY(EditAnywhere, Category = "Grapple physics", meta = (AllowPrivateAccess = "true"))
	float maxDistanceGrappling = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Grapple physics", meta = (AllowPrivateAccess = "true"))
	float velocity = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "Grapple physics", meta = (AllowPrivateAccess = "true"))
	float offsetStop = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Grapple physics", meta = (AllowPrivateAccess = "true"))
	float offsetBlockingObject = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Grapple physics", meta = (AllowPrivateAccess = "true"))
	float stopScaleVelocity = 0.0f;

	UPROPERTY(EditAnywhere, Category = "AutoGrapple", meta = (AllowPrivateAccess = "true"))
	float maxDot = 1.0f;

	UPROPERTY(EditAnywhere, Category = "AutoGrapple", meta = (AllowPrivateAccess = "true"))
	float minDot = 0.0f;

	UPROPERTY(EditAnywhere, Category = "physics", meta = (AllowPrivateAccess = "true"))
	float minDistance = 100.0f;

	UPROPERTY()
	bool IsFiring;

	// Called when the game starts
	virtual void BeginPlay() override;

	void CheckElementTargetable();

	UFUNCTION()
	void AttractCharacter();

	UFUNCTION()
	void PlayerIsNear();

public:
	UPROPERTY()
	FVector IKposition;
	// Sets default values for this component's properties
	UGrappleComponent();

	UFUNCTION()
	FORCEINLINE bool GetFiring() { return IsFiring; }

	UFUNCTION(BlueprintCallable)
	void GoToDestination(bool _isAssisted);

	UPROPERTY(BlueprintReadOnly)
	bool IsTargetingGrapple;

	UFUNCTION()
	void Cancel();

	UFUNCTION(BlueprintCallable)
	void SetIKArm(FVector& _lookAt, bool& _isBlend);

	FORCEINLINE bool& GetIsFiring() { return IsFiring; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector& GetIKPosition() { return IKposition; };

	FORCEINLINE const FVector& GetDirection() { return mDirection; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class AProjectileHand* GetProjectile() { return currentProjectile; };

	UFUNCTION()
	FVector GetHandPosition();

	UFUNCTION()
	FVector GetVirtualRightHandPosition();

	UFUNCTION()
	FVector GetVirtualLeftHandPosition();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class AActor* GetClosestGrapplingHook() { return ClosestGrapplingHook; };

	FORCEINLINE class USwingPhysic* GetSwingPhysics() { return swingPhysic; };

	UFUNCTION()
	void UpdateIKArm();

	UFUNCTION()
	FORCEINLINE class AGolemProjectCharacter* GetCharacter() { return mCharacter; };

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Implicit Delete SwingPhysics var 
	void StopSwingPhysics();

	void CheckGround();
};
