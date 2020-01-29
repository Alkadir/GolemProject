// Fill out your copyright notice in the Description page of Project Settings.


#include "DartTrap.h"
#include "Engine/World.h"
#include "Objects/Projectile.h"

void ADartTrap::BeginPlay()
{
	Super::BeginPlay();
	timerCooldown = cooldown;
	if (alwaysActive)
	{
		Activate(nullptr);
	}
}

void ADartTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timerCooldown -= DeltaTime;
	if (!isActivate)
	{
		return;
	}
	if (timerCooldown <= 0.f)
	{
		timerCooldown = cooldown;
		GetWorld()->SpawnActor<AProjectile>(projectilePrefab, GetActorLocation(), FRotator::ZeroRotator)->LaunchProjectile(this, GetActorForwardVector() * speedProjectile, damage);
	}
}