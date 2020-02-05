// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Activable.h"
#include "Trap.generated.h"

UCLASS()
class GOLEMPROJECT_API ATrap : public AActor, public IActivable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrap();

protected:
	// Called when the game starts or when spawned
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Trap")
		bool isActivate = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Trap")
		bool alwaysActive = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Trap")
		int damage = 1;



public:	


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trap")
		const bool Activate(AActor* caller);
	virtual const bool Activate_Implementation(AActor* caller) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trap")
		const bool Desactivate(AActor* caller);
	virtual const bool Desactivate_Implementation(AActor* caller) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trap")
		const bool Switch(AActor* caller) ;
	virtual const bool Switch_Implementation(AActor* caller) override;
};
