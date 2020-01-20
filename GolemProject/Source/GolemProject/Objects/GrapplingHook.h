// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Targetable.h"
#include "GrapplingHook.generated.h"

UCLASS()
class GOLEMPROJECT_API AGrapplingHook : public AActor, public ITargetable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrapplingHook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Targetable")
		void CreateHUD() const;
	virtual void CreateHUD_Implementation() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
