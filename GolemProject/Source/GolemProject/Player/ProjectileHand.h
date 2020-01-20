// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileHand.generated.h"

UCLASS()
class GOLEMPROJECT_API AProjectileHand : public AActor
{
	GENERATED_BODY()

	FVector direction;
	class UStaticMeshComponent* meshComponent;
	bool bIsColliding;

	UPROPERTY(EditAnywhere, Category = "physics", meta = (AllowPrivateAccess = "true"))
	float velocity = 3000.0f;
public:
	// Sets default values for this actor's properties
	AProjectileHand();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
public:
	const bool& IsColliding();
	void LaunchProjectile(const FVector& _direction);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
