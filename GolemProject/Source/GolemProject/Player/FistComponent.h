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
	class UCameraComponent* mCamera;
	class AGolemProjectCharacter* mCharacter;
	FVector mDirection;
	float accuracy = 100000.0f;
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


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly)
	bool IsTargetingFist;

	UFUNCTION()
	void UpdateIKArm();

	UPROPERTY()
	FVector IKposition;

	FORCEINLINE const FVector& GetDirection() { return mDirection; };

	FORCEINLINE class AFistProjectile* GetProjectile() { return currentProjectile; };
};
