// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressurePlate.generated.h"

class UBoxComponent;

UCLASS()
class GOLEMPROJECT_API APressurePlate : public AActor
{
	GENERATED_BODY()

private:
	FTimerHandle pressedTimerHandle;
	FTimerHandle releasedTimerHandle;

protected:
	UBoxComponent* boxCollider = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Collision")
		FVector colliderSize = FVector::OneVector * 50.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Collision")
		FVector boxOffset = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	bool isPressed = false;

public:
	APressurePlate();

private:
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnPressed();

	UFUNCTION()
		void OnReleased();

protected:
	virtual void BeginPlay() override;

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif //WITH_EDITOR
};
