// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrappleComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOLEMPROJECT_API UGrappleComponent : public UActorComponent
{
	GENERATED_BODY()
	class UCameraComponent* mCamera;
	class AGolemProjectCharacter* mCharacter;
	class USkeletalMeshComponent* mSkeletalMesh;
	FVector mDestination;
	FVector mDirection;
	int32 mIdBone;
	class AProjectileHand* currentProjectile;
	AActor* ClosestGrapplingHook;
	class AGolemProjectGameMode* GameMode;
	class APlayerCameraManager* PlayerCameraManager;

	float accuracy = 100000.0f;

protected:
	UPROPERTY(EditAnywhere, Category = "projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectileHand> handProjectileClass;

	UPROPERTY(EditAnywhere, Category = "physics", meta = (AllowPrivateAccess = "true"))
	float maxDistance = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "physics", meta = (AllowPrivateAccess = "true"))
	float velocity = 200.0f;

	UPROPERTY(EditAnywhere, Category = "physics", meta = (AllowPrivateAccess = "true"))
	float offsetStop = 200.0f;

	// Called when the game starts
	virtual void BeginPlay() override;

	void CheckElementTargetable();

public:
	UPROPERTY()
	FVector IKposition;
	// Sets default values for this component's properties
	UGrappleComponent();

	UFUNCTION(BlueprintCallable)
	void GoToDestination(bool _isAssisted);

	UFUNCTION()
	void Cancel();

	UFUNCTION(BlueprintCallable)
	void SetIKArm(FVector& _lookAt, bool& _isBlend);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector& GetIKPosition() { return IKposition; };

	FORCEINLINE const FVector& GetDirection() { return mDirection; };

	FORCEINLINE class AProjectileHand* GetProjectile() { return currentProjectile; };

	UFUNCTION()
	FVector GetHandPosition();

	UFUNCTION()
	void PlayerIsNear();

	UFUNCTION()
	void UpdateIKArm();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
