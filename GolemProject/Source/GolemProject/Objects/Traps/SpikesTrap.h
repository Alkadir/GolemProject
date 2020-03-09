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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInstancedStaticMeshComponent* ArraySpikes;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxDmgTrigger;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxTrigger;

	void BeginPlay() override;

	void Tick(float _deltaTime) override;

	FVector EndLocation;
private:
	UFUNCTION()
	void OverlapDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapActivation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool IsTriggered;

};
