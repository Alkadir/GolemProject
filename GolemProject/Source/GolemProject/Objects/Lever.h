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
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly)
		TArray<AActor*> objectsToActivate;
	UPROPERTY(EditInstanceOnly)
		EActivationType activationType;


public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		const bool Interact(AActor* caller);
	virtual const bool Interact_Implementation(AActor* caller) override;

};
