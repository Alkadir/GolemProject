// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "PushableBloc.generated.h"

class UBoxComponent;
class AGolemProjectCharacter;

UCLASS()
class GOLEMPROJECT_API APushableBloc : public AActor, public IInteractable
{
	GENERATED_BODY()

private:
	AGolemProjectCharacter* playerActor = nullptr;
	bool isUsed = false;

protected:
	UBoxComponent* boxCollider = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Collision")
		FVector colliderSize = FVector::OneVector * 50.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Collision")
		FVector boxOffset = FVector::ZeroVector;

public:
	APushableBloc();

private:
	void BeginPlay();

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		const bool Interact(AActor* caller);
	virtual const bool Interact_Implementation(AActor* caller) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif //WITH_EDITOR
};
