// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/EngineTypes.h"
#include "PlayerManager.generated.h"
/**
 * 
 */
UCLASS()
class GOLEMPROJECT_API UPlayerManager : public UObject
{
	GENERATED_BODY()
private :
	void ResetInvulnerability();

public:
	UPlayerManager(const FObjectInitializer& ObjectInitializer);
	/*inline static UPlayerManager* GetInstance()
	{
		static UPlayerManager Instance;

		return &Instance;
	};*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	inline int GetLife() const
	{
		return Life;
	};

	inline FVector GetLastPositionGrounded() const
	{
		return LastPositionGrounded;
	};

	inline void SetLastPositionGrounded(FVector _lastPositionGrounded)
	{
		LastPositionGrounded = _lastPositionGrounded;
	};

	FVector GetPositionCheckPoint() const
	{
		return PositionCheckPoint;
	};

	void SetPositionCheckPoint(FVector _positionCheckPoint)
	{
		PositionCheckPoint = _positionCheckPoint;
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
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void RespawnPlayerAfterDeath();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void RespawnPlayerAfterFall();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Initialize(int _maxLife, float _timerInvulnerability, class AGolemProjectCharacter* _player);

private:
	int MaxLife;
	int Life;
	FVector LastPositionGrounded;
	FVector PositionCheckPoint;
	float TimerInvulnerability;
	bool CanTakeDamage;
	FTimerHandle TimerHandlerInvul;
	class AGolemProjectCharacter* Player;

};
