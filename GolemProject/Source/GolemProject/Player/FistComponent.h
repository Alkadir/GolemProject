// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FistComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOLEMPROJECT_API UFistComponent : public UActorComponent
{
	GENERATED_BODY()

	class UWorld* world;
	class AFistProjectile* currentProjectile;
	class USkeletalMeshComponent* mSkeletalMesh;
	class UCameraComponent* mCamera;
	class AGolemProjectCharacter* mCharacter;
	FVector mDirection;
	float accuracy = 100000.0f;
	int32 mIdBone;
	bool CanFire;
	FTimerHandle TimerHandleFire;

private :
	void ResetFire();
public:	
	// Sets default values for this component's properties
	UFistComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AFistProjectile> fistProjectileClass;

	UFUNCTION(BlueprintCallable)
	void SetIKArm(FVector& _lookAt, bool& _isBlend);

	UPROPERTY(EditAnywhere)
	float TimerFire = 1.0f;

	UFUNCTION()
	void UpdateIKArm();

	UPROPERTY()
	FVector IKposition;

	bool DisplayEnable;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly)
	bool IsTargetingFist;

	UFUNCTION(BlueprintCallable)
	void GoToDestination();

	UFUNCTION()
	void DisplayTrajectory();

	UFUNCTION()
	FVector GetHandPosition();

	FORCEINLINE const FVector& GetDirection() { return mDirection; };

	FORCEINLINE class AFistProjectile* GetProjectile() { return currentProjectile; };
};
