// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AProjectile::AProjectile(const FObjectInitializer& OI)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(OI.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root")));
	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void AProjectile::LaunchProjectile_Implementation(AActor* _launcher, FVector velocity, int _damage, float gravityScale)
{
	launcher = _launcher;
	damage = _damage;
	if (projectileMovement)
	{
		projectileMovement->Velocity = velocity;
		projectileMovement->ProjectileGravityScale = gravityScale;
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

