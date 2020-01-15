// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrappleComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOLEMPROJECT_API UGrappleComponent : public UActorComponent
{
	GENERATED_BODY()

	class APawn* mPawn;
	FVector mDestination;
	FVector mDirection;
	bool bIsGrappling;

public:
	// Sets default values for this component's properties
	UGrappleComponent();
protected:
	
	UPROPERTY(EditAnywhere, Category = "physics", meta = (AllowPrivateAccess = "true"))
	float velocity = 200.0f;

	UPROPERTY(EditAnywhere, Category = "physics", meta = (AllowPrivateAccess = "true"))
	float offsetStop = 500.0f;
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void GoToDestination();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
