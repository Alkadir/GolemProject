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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float timeToSetupPush = 0.5f;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		bool isPushingObject = false;
	UPROPERTY(BlueprintReadWrite)
		FVector pushingDirection = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
		FVector pushingPosition = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
		FRotator pushingRotation = FRotator::ZeroRotator;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		bool GetIsPushingObject() { return isPushingObject; }
	UFUNCTION(BlueprintCallable)
		FVector GetPushingDirection() { return pushingDirection; }
	UFUNCTION(BlueprintCallable)
		FVector GetPushingPosition() { return pushingPosition; }
	UFUNCTION(BlueprintCallable)
		FRotator GetPushingRotation() { return pushingRotation; }
	UFUNCTION(BlueprintCallable)
		void SetIsPushingObject(bool isPushing) { isPushingObject = isPushing; }

	UFUNCTION(BlueprintCallable)
	void PushBloc(FVector newPushingDirection, FVector newPushingPosition, FRotator newPushingRotation);

	UFUNCTION(BlueprintCallable)
	void StopPushBloc();

	UPROPERTY(BlueprintAssignable)
		FPushing OnPushingObject;
	UPROPERTY(BlueprintAssignable)
		FPushing OnStopPushingObject;


};
