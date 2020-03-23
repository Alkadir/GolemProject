// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileHand.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Player/GrappleComponent.h"
#include "Helpers/HelperLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

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
			dir /= dir.Size();
			meshComponent->SetPhysicsLinearVelocity(dir * velocity);
		}
	}
	else
	{
		if (!bIsColliding)
			meshComponent->SetPhysicsLinearVelocity(direction * velocity);
		else
			meshComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	}
}

void AProjectileHand::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bIsComingBack && HitComponent != nullptr && OtherActor != this)
	{
		bIsColliding = true;
		UPhysicalMaterial* physMat;
		if (Hit.GetComponent()->GetMaterial(0) != nullptr)
		{
			physMat = Hit.GetComponent()->GetMaterial(0)->GetPhysicalMaterial();
			if (physMat != nullptr)
			{
				if (!bIsAssisted && physMat->SurfaceType == SurfaceType1)
				{
					bIsGrapplingPossible = true;
					return;
				}
				else if (bIsAssisted && physMat->SurfaceType == SurfaceType3)
				{
					bIsSwingingPossible = true;
					return;
				}
			}
		}
		bIsComingBack = true;
	}
}



