// Fill out your copyright notice in the Description page of Project Settings.


#include "FistProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AFistProjectile::AFistProjectile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComp = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	RootComponent = RootComp;
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AFistProjectile::BeginPlay()
{
	Super::BeginPlay();	
	ProjectileComponent->MaxSpeed = Speed;
	ProjectileComponent->InitialSpeed = 0.0f;
	
	//ProjectileComponent->UpdatedComponent = MeshComponent;
}

// Called every frame
void AFistProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFistProjectile::LaunchFist(const FVector& _direction)
{
	Direction = _direction;
	ProjectileComponent->Velocity = Direction * Speed;
}

