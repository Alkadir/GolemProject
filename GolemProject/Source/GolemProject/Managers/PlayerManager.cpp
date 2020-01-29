// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerManager.h"
#include "GolemProjectCharacter.h" 

UPlayerManager::UPlayerManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPlayerManager::InflictDamage(int _damage)
{
	Life -= _damage;
	CanTakeDamage = false;
	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().SetTimer(TimerHandlerInvul, this, &UPlayerManager::ResetInvulnerability, TimerInvulnerability, true);
	}
	if (Life <= 0)
	{
		Life = 0;
		RespawnPlayerAfterDeath();
		//kill player
	}
}

void UPlayerManager::RespawnPlayerAfterDeath()
{
	if (Player != nullptr)
		Player->SetActorLocation(PositionCheckPoint);
}

void UPlayerManager::RespawnPlayerAfterFall()
{
	if (Player != nullptr)
		Player->SetActorLocation(LastPositionGrounded);
}

void UPlayerManager::Initialize(int _maxLife, float _timerInvulnerability, AGolemProjectCharacter* _player)
{
}

void UPlayerManager::ResetInvulnerability()
{
	CanTakeDamage = true;
}
