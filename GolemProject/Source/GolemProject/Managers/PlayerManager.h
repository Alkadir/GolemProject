// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class GOLEMPROJECT_API PlayerManager
{
public:
	inline static PlayerManager* GetInstance()
	{
		static PlayerManager Instance;

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
	float TimerInvulnerability;
	bool CanTakeDamage;
	class AGolemProjectCharacter* Player;

private:
	PlayerManager();
	~PlayerManager();
};
