// Golem Project - Créajeux 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "PortalLevel.generated.h"

UCLASS()
class GOLEMPROJECT_API APortalLevel : public AActor, public IInteractable
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
		class UStaticMeshComponent* ColliderPortal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* BoxDetectionCloseEnough;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* CenterToReach;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UChildActorComponent* CameraComp;

	UFUNCTION()
	void OnBeginOverlapDetection(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapDetection(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintReadOnly)
	bool IsPlayerCloseEnough;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString NameLevel;

	UFUNCTION(BlueprintImplementableEvent)
	void IsCharacterCloseEnoughEvent(bool _closeEnough);

	UFUNCTION(BlueprintImplementableEvent)
	void CharacterTriggeredPortalEvent();

	UFUNCTION()
	void ActivatePortal();

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeCamera();

	UFUNCTION(BlueprintImplementableEvent)
	bool IsLevelUnlocked();

	FVector lastLocationRing;

	bool isPortalActivate;

	float timerLerp;

	UPROPERTY(EditAnywhere)
	float speedLerp;

	bool hasToTargetCharacter;

	class AGolemProjectCharacter* targetCharacter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		const bool Interact(AActor* caller);
	virtual const bool Interact_Implementation(AActor* caller) override;

};
