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


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		const bool Interact(const AActor* caller);
	virtual const bool Interact_Implementation(const AActor* caller) override;

};
