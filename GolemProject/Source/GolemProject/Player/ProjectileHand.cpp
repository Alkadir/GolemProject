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
	ProjectileComponent = FindComponentByClass<UProjectileMovementComponent>();
	ProjectileComponent->MaxSpeed = MaxSpeed;
	ProjectileComponent->InitialSpeed = 0.0f;
	ProjectileComponent->UpdatedComponent = meshComponent;

	bIsColliding = false;
	bIsComingBack = false;
	meshComponent->OnComponentHit.AddDynamic(this, &AProjectileHand::OnHit);
}

void AProjectileHand::LaunchProjectile(const FVector& _direction, UGrappleComponent* _grapple)
{
	direction = _direction;
	grappleComponent = _grapple;
	ProjectileComponent->Velocity = direction * velocity;
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
			ProjectileComponent->Velocity = dir * velocity;
		}
	}
	else
	{
		if (!bIsColliding)
			ProjectileComponent->Velocity = direction * velocity;
		else
			ProjectileComponent->Velocity = FVector::ZeroVector;
	}
}

void AProjectileHand::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//change direction projectile, maybe add timer when the projectile is reallly blocked
	if (bIsComingBack && OtherActor != this)
	{
		FVector changeDir = OtherActor->GetActorLocation() - GetActorLocation();
		changeDir /= changeDir.Size();
		ProjectileComponent->Velocity = changeDir*velocity;
	}

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



