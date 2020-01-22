// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerManager.generated.h"

/**
 * 
 */
UCLASS()
class GOLEMPROJECT_API UPlayerManager : public UObject
{
	GENERATED_BODY()
public:
	inline static UPlayerManager* GetInstance()
	{
		static UPlayerManager Instance;

		return &Instance;
	};

	inline int GetLife() const
	{
		return Life;
	};
	void InflictDamage(int _damage);

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

	class AGolemProjectCharacter* GetPlayer() const
	{
		return Player;
	};

	void SetPlayer(class AGolemProjectCharacter* _player)
	{
		Player = _player;
	}


private:
	int MaxLife;
	int Life;
	FVector LastPositionGrounded;
	FVector PositionCheckPoint;
	//FTimerHandler TimerHandlerInvul;
	float TimerInvulnerability;
	bool CanTakeDamage;
	class AGolemProjectCharacter* Player;

};
