// Golem Project - Créajeux 2020

#pragma once

#include "CoreMinimal.h"
#include "Objects/Traps/Trap.h"
#include "LazorTrap.generated.h"

/**
 * 
 */
UCLASS()
class GOLEMPROJECT_API ALazorTrap : public ATrap
{
	GENERATED_BODY()

		ALazorTrap();
	
protected:
	UPROPERTY(EditAnywhere)
		float MaxDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* LazerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootComp;

	void BeginPlay() override;

	void Tick(float _deltaTime) override;
private:
	UFUNCTION()
		void OverlapDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
