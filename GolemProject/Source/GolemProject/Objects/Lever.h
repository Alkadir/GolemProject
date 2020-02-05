// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Interfaces/Activable.h"
#include "Lever.generated.h"

UCLASS()
class GOLEMPROJECT_API ALever : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALever();

protected:
	UPROPERTY(EditInstanceOnly)
		TArray<AActor*> objectsToActivate;
	UPROPERTY(EditInstanceOnly)
		EActivationType activationType;

	class UStaticMeshComponent* collider = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		const bool Interact(AActor* caller);
	virtual const bool Interact_Implementation(AActor* caller) override;

};
