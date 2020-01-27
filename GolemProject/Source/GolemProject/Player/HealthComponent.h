// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOLEMPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = "true"))
	int MaxLife;
	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = "true"))
	int Life;
	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float TimerInvulnerability;
	bool CanTakeDamage;
	FTimerHandle TimerHandlerInvul;
	class AGolemProjectCharacter* Player;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		inline int GetLife() const
	{
		return Life;
	};

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		class AGolemProjectCharacter* GetPlayer() const
	{
		return Player;
	};

	void SetPlayer(class AGolemProjectCharacter* _player)
	{
		Player = _player;
	}

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void InflictDamage(int _damage);

private:
	void ResetInvulnerability();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
