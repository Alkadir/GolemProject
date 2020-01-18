// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DashComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOLEMPROJECT_API UDashComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDashComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class AGolemProjectCharacter* m_character;

	class UCharacterMovementComponent* CharacterMovementCmpt;

	UPROPERTY(EditAnywhere, Category = VelocityDash)
		float ForceDash = 2000.0f;

	UPROPERTY(EditAnywhere, Category = VelocityDash)
		float ForceAfterDash = 500.0f;

	UPROPERTY(EditAnywhere, Category = VelocityDash)
		float TimerStopDash = 0.2f;

	UPROPERTY(EditAnywhere, Category = TimerDash)
		float CDDash = 1.0f;

	FVector CurrentVelocity;

	FVector CurrentDirection;

	//commentaire
	bool m_canDash;

	bool HasDashInAir;

	float m_groundFriction;

	FTimerHandle m_loopTimer;

	FTimerHandle m_timerDash;


	void StopDash();

	void CanRedashDash();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Dash(FVector _direction);

	UFUNCTION(BlueprintCallable)
		void ResetDashInAir();
		
};
