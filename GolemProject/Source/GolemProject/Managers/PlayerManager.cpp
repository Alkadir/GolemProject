// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerManager.h"
#include "GolemProjectCharacter.h"

void PlayerManager::InflictDamage(int _damage)
{
	Life -= _damage;
	CanTakeDamage = false;
	if (Life <= 0)
	{
		//kill player
	}
}

PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{
}
