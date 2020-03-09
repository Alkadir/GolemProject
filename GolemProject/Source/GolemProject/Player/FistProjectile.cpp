// Fill out your copyright notice in the Description page of Project Settings.


#include "FistProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Helpers/HelperLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

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
		UPhysicalMaterial* physMat;
		if (Hit.GetComponent()->GetMaterial(0) != nullptr)
		{
			physMat = Hit.GetComponent()->GetMaterial(0)->GetPhysicalMaterial();
			if (physMat != nullptr && physMat->SurfaceType == SurfaceType2)
			{
				if (ProjectileComponent != nullptr)
					ProjectileComponent->bShouldBounce = true;
				BounceMovement(Hit.ImpactNormal);
			}
			else
			{
				if (ProjectileComponent != nullptr)
					ProjectileComponent->bShouldBounce = false;
			}
		}

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
}

void AFistProjectile::DestroyFist()
{
	Destroy();
}

void AFistProjectile::BounceMovement(FVector _normal)
{
	if (ProjectileComponent != nullptr)
	{
		FVector direction = ProjectileComponent->Velocity.GetSafeNormal();
		FVector newDirection = direction.MirrorByVector(_normal).GetSafeNormal();
		ProjectileComponent->Velocity = newDirection * Speed;
	}
}

