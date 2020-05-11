// Golem Project - Créajeux 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalLevel.generated.h"

UCLASS()
class GOLEMPROJECT_API APortalLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalLevel();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* TriggerChangeLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* BoxDetectionCloseEnough;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* Root;

	UFUNCTION()
	void OnBeginOverlapDetection(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapDetection(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadOnly)
	bool IsPlayerCloseEnough;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString NameLevel;

	UFUNCTION(BlueprintImplementableEvent)
	void IsCharacterCloseEnoughEvent(bool _closeEnough);

	UFUNCTION(BlueprintImplementableEvent)
	void CharacterTriggeredPortalEvent();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
