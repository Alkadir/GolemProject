// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Activable.h"
#include "Door.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EOpenDoorBy : uint8
{
	Animation		UMETA(DisplayName = "Animation"),
	Movement	 	UMETA(DisplayName = "Movement")
};

UCLASS()
class GOLEMPROJECT_API ADoor : public AActor, public IActivable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor(const FObjectInitializer& OI);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		EOpenDoorBy openBy;

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* destination;

	UPROPERTY(EditAnywhere)
		bool isActivate;

	UPROPERTY(EditAnywhere)
		float timeToMove;


	FVector startPos;
	FVector destinationPos;
	float timerLerp;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Activable")
		const bool Activate(AActor* caller);
	virtual const bool Activate_Implementation(AActor* caller) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Activable")
		bool Desactivate(AActor* caller);
	virtual const bool Desactivate_Implementation(AActor* caller) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Activable")
		bool Switch(AActor* caller);
	virtual const bool Switch_Implementation(AActor* caller) override;


};
