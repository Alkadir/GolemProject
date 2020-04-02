// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DashComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VelocityDash)
		float ForceDash = 1300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VelocityDash)
		float ForceDashInTheAir = 666.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VelocityDash)
		float ForceAfterDash = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VelocityDash)
		float TimerStopDash = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TimerDash)
		float CDDash = 1.0f;

	FVector CurrentVelocity;

	FVector CurrentDirection;

	bool m_canDash;

	bool isDashing;

	bool HasDashInAir;

	FTimerHandle m_loopTimer;

	FTimerHandle m_timerDash;

	void StopDash();

	void CanRedashDash();

public:

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsDashing() { return isDashing; }

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void Dash(FVector _direction);

	UFUNCTION(BlueprintCallable)
		void ResetDashInAir();

};
