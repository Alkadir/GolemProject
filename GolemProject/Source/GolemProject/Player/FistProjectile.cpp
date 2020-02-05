// Fill out your copyright notice in the Description page of Project Settings.


#include "FistProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
AFistProjectile::AFistProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFistProjectile::BeginPlay()
{
	Super::BeginPlay();	
	ProjectileComponent = FindComponentByClass<UProjectileMovementComponent>();
	MeshComponent = FindComponentByClass <UStaticMeshComponent>();
	ProjectileComponent->MaxSpeed = Speed;
	ProjectileComponent->InitialSpeed = 0.0f;
	ProjectileComponent->UpdatedComponent = MeshComponent;
	MeshComponent->OnComponentHit.AddDynamic(this, &AFistProjectile::OnHit);
}

void AFistProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HitComponent != nullptr && OtherActor != nullptr && OtherComponent != nullptr)
	{
		if (UWorld * world = GetWorld())
		{
			world->GetTimerManager().SetTimer(TimerHandleFXDisappear, this, &AFistProjectile::Event_DestructionFistFX_BP, TimerDisappear - 1.0f, false);
			world->GetTimerManager().SetTimer(TimerHandleDisappear, this, &AFistProjectile::DestroyFist, TimerDisappear, false);
		}
	}
}

// Called every frame
void AFistProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFistProjectile::LaunchFist(const FVector& _direction, bool _shouldBounce)
{
	Direction = _direction;
	ProjectileComponent->Velocity = Direction * Speed;
	ProjectileComponent->bShouldBounce = true;
}

void AFistProjectile::DestroyFist()
{
	Destroy();
}

