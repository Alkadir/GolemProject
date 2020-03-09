// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "PushableBloc.generated.h"

class AGolemProjectCharacter;

UCLASS()
class GOLEMPROJECT_API APushableBloc : public AActor, public IInteractable
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* mesh;

	AGolemProjectCharacter* playerActor = nullptr;
	bool isUsed = false;
	UPROPERTY(BlueprintReadWrite)
		FVector sizeBlock = FVector::OneVector;
	UPROPERTY(BlueprintReadWrite)
		FVector pushingDirection = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
		FVector pushingPosition = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
		FRotator pushingRotation = FRotator::ZeroRotator;
public:
	APushableBloc();

	UFUNCTION(BlueprintCallable)
		FVector GetPushingDirection() { return pushingDirection; }
	UFUNCTION(BlueprintCallable)
		FVector GetPushingPosition() { return pushingPosition; }
	UFUNCTION(BlueprintCallable)
		FRotator GetPushingRotation() { return pushingRotation; }

private:

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		const bool Interact(AActor* caller);
	virtual const bool Interact_Implementation(AActor* caller) override;

};
