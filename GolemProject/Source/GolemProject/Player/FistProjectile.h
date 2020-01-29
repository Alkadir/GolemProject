// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FistProjectile.generated.h"

UCLASS()
class GOLEMPROJECT_API AFistProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFistProjectile(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	float Speed;
	UPROPERTY(BlueprintReadOnly)
	bool IsColliding;

	UPROPERTY(EditAnywhere, Category = "physics", meta = (AllowPrivateAccess = "true"))
	float maxDistance = 3000.0f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Blueprintcallable)
	FORCEINLINE class UProjectileMovementComponent* GetProjectileComponent() { return ProjectileComponent; };
	
	UFUNCTION(Blueprintcallable)
	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() { return MeshComponent; };

	UFUNCTION(Blueprintcallable)
	void LaunchFist(const FVector& _direction);
private :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Fist, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Fist, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Fist, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootComp;
	FVector Direction;
};
