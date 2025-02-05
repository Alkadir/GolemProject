// Golem Project - Créajeux 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PushingComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPushing);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GOLEMPROJECT_API UPushingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPushingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	class UCharacterMovementComponent* characterMovementCmpt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float timeToSetupPush = 0.5f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float speedWhenPushing = 150.f;
	float lastSpeed;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		bool isPushingObject = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		bool isStartingPushingObject = false;
	UPROPERTY(BlueprintReadWrite)
		FVector pushingDirection = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
		FVector pushingPosition = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
		FRotator pushingRotation = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadWrite)
		FVector blockOffsetPosition = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
		FVector sizeBlock = FVector::ZeroVector;

	class APushableBloc* block;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		bool GetIsPushingObject(bool startingToo = true);
	UFUNCTION(BlueprintCallable)
		bool GetIsStartingPushingObject() { return isStartingPushingObject; }
	UFUNCTION(BlueprintCallable)
		FVector GetPushingDirection() { return pushingDirection; }
	UFUNCTION(BlueprintCallable)
		FVector GetPushingPosition() { return pushingPosition; }
	UFUNCTION(BlueprintCallable)
		FRotator GetPushingRotation() { return pushingRotation; }
	UFUNCTION(BlueprintCallable)
		FVector GetBlockOffsetPosition() { return blockOffsetPosition; }
	UFUNCTION(BlueprintCallable)
		FVector GetSizeBlock() { return sizeBlock; }

	UFUNCTION(BlueprintCallable)
		void SetBlockOffsetPosition(FVector newOffset) { blockOffsetPosition = newOffset; }
	UFUNCTION(BlueprintCallable)
		void SetSizeBlock(FVector newSize) { sizeBlock = newSize; }
	UFUNCTION(BlueprintCallable)
		void SetBlock(APushableBloc* _block);


	UFUNCTION(BlueprintCallable)
		void SetIsPushingObject(bool isPushing) { isPushingObject = isPushing; }
	UFUNCTION(BlueprintCallable)
		void SetIsStartingPushingObject(bool isStartingPushing);
	UFUNCTION(BlueprintCallable)
		void StartPushingObject();
	UFUNCTION(BlueprintCallable)
		void EndPushingObject();
	UFUNCTION(BlueprintCallable)
		void StartStartingPushingObject();
	UFUNCTION(BlueprintCallable)
		void EndStartingPushingObject();

	UFUNCTION(BlueprintCallable)
		void PushBloc(FVector newPushingDirection, FVector newPushingPosition, FRotator newPushingRotation);

	UFUNCTION(BlueprintCallable)
		void StopPushBloc();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FPushing OnStartStartingPushingObject;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FPushing OnEndStartingPushingObject;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FPushing OnStartPushingObject;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FPushing OnEndPushingObject;


};
