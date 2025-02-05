// Golem Project - Créajeux 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BumperAir.generated.h"

UCLASS()
class GOLEMPROJECT_API ABumperAir : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABumperAir();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshBeam;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SphereTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	float ForceLaunch;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
