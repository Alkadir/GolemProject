// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrappleComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GOLEMPROJECT_API UGrappleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrappleComponent();

protected:
	APawn* mPawn;

	UPROPERTY(EditAnywhere, Category = "physics")
	FVector launchVelocity;

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void GoToDestination(FVector destination);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
