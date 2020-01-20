// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileHand.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Player/GrappleComponent.h"
#include "Helpers/HelperLibrary.h"
// Sets default values
AProjectileHand::AProjectileHand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectileHand::BeginPlay()
{
	Super::BeginPlay();
	meshComponent = FindComponentByClass<UStaticMeshComponent>();

	bIsColliding = false;
	bIsComingBack = false;
	meshComponent->OnComponentHit.AddDynamic(this, &AProjectileHand::OnHit);
}

void AProjectileHand::LaunchProjectile(const FVector& _direction, UGrappleComponent* _grapple)
{
	direction = _direction;
	grappleComponent = _grapple;
}

// Called every frame
void AProjectileHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsComingBack)
	{
		if (grappleComponent)
		{
			FVector dir = grappleComponent->GetHandPosition() - meshComponent->GetComponentLocation();
			dir.Normalize();
			meshComponent->SetPhysicsLinearVelocity(dir * velocity);
		}
	}
	else
	{
		if (meshComponent && meshComponent->IsSimulatingPhysics())
			meshComponent->SetPhysicsLinearVelocity(direction * velocity);
	}
}

void AProjectileHand::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bIsComingBack && HitComponent != nullptr && OtherActor != this)
	{
		meshComponent->SetSimulatePhysics(false);
		bIsColliding = true;
	}
}

const bool& AProjectileHand::IsColliding()
{
	return bIsColliding;
}


