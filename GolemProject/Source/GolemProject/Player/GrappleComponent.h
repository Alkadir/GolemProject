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
	class SwingPhysics* swingPhysics = nullptr;
	class AActor* HelperAiming;

	FVector mDestination;
	FVector mDirection;

	int32 mIdBone;
	bool bIsAssisted = false;
	float accuracy = 100000.0f;
	
protected:
	UPROPERTY(EditAnywhere, Category = "projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectileHand> handProjectileClass;

	UPROPERTY(EditAnywhere, Category = Help)
	TSubclassOf<class AActor> HelperAimingClass;

	UPROPERTY(EditAnywhere, Category = "physics", meta = (AllowPrivateAccess = "true"))
	float maxDistance = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "physics", meta = (AllowPrivateAccess = "true"))
	float velocity = 200.0f;

	UPROPERTY(EditAnywhere, Category = "physics", meta = (AllowPrivateAccess = "true"))
	float offsetStop = 200.0f;

	UPROPERTY(EditAnywhere, Category = "physics", meta = (AllowPrivateAccess = "true"))
	float offsetBlockingObject = 200.0f;

	UPROPERTY(EditAnywhere, Category = "physics", meta = (AllowPrivateAccess = "true"))
	float stopScaleVelocity = 0.4f;

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

	FORCEINLINE class AProjectileHand* GetProjectile() { return currentProjectile; };

	UFUNCTION()
	FVector GetHandPosition();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class AActor* GetClosestGrapplingHook() { return ClosestGrapplingHook; };

	FORCEINLINE class SwingPhysics* GetSwingPhysics() { return swingPhysics; };

	UFUNCTION()
	void UpdateIKArm();

	UFUNCTION()
	FORCEINLINE class AGolemProjectCharacter* GetCharacter() { return mCharacter; };

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Implicit Delete SwingPhysics var 
	void StopSwingPhysics();
};
