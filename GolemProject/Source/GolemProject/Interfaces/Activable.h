// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Activable.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UActivable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GOLEMPROJECT_API IActivable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Activable")
		bool Activate(AActor* caller) const;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Activable")
		bool Desactivate(AActor* caller) const;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Activable")
		bool Switch(AActor* caller) const;
};
