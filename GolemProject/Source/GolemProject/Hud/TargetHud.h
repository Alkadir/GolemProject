// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "TargetHud.generated.h"

/**
 * 
 */
UCLASS()
class GOLEMPROJECT_API UTargetHud : public UWidgetComponent
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, Category = " Hud Sight")
		TSubclassOf<class UUserWidget> widgetSightClass;

	protected:
	class UUserWidget* mWidgetSight;
	virtual void BeginPlay() override;
};
