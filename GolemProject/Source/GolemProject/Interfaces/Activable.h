// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Activable.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EActivationType : uint8
{
	Activate		UMETA(DisplayName = "Activate"),
	Desactivate 	UMETA(DisplayName = "Desactivate"),
	Switch			UMETA(DisplayName = "Switch")
};

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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Activable")
		bool Activate(const AActor* caller) const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Activable")
		bool Desactivate(const AActor* caller) const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Activable")
		bool Switch(const AActor* caller) const;
};
