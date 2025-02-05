// Golem Project - Créajeux 2020

#pragma once

#include "CoreMinimal.h"
#include "Objects/Traps/Trap.h"
#include "SpikesTrap.generated.h"

/**
 * 
 */
UCLASS()
class GOLEMPROJECT_API ASpikesTrap : public ATrap
{
	GENERATED_BODY()

		ASpikesTrap();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshSpikes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxTrigger;

	void BeginPlay() override;

	void Tick(float _deltaTime) override;

	FVector EndLocation;

	FVector StartingLocation;

	UPROPERTY(EditAnywhere)
	float TimerTrigger;

	float TimerBeforeTrigger;

	float TimerSpikes;

	UPROPERTY(EditAnywhere, Category = "Auto Spike")
	bool ActivationAuto;

	UPROPERTY(EditAnywhere, Category = "Auto Spike")
	float TimeBetweenActivation;

	UPROPERTY(EditAnywhere, Category = "Auto Spike")
	float TimeSpikesOut;

	UPROPERTY(EditAnywhere, Category = "Auto Spike")
	float Interval = 0.0f;

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Always out")
	bool AlwaysOut;

	UPROPERTY(EditAnywhere)
	float offset = 50.0f;
private:
	UFUNCTION()
	void OverlapDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapActivation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlapActivation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool IsTriggered;

	bool RetractSpike;

	bool IsPlayerStillHere;
};
