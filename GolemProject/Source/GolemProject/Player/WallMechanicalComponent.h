// Golem Project - Créajeux 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WallMechanicalComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOLEMPROJECT_API UWallMechanicalComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	class UWorld* world;
	class AGolemProjectCharacter* mCharacter;
	TArray<class AActor*> ActorsToIgnore;
	FVector DirectionJump;
	FVector PositionFreeze;
	FRotator RotationToReach;
	float TimerRotation;
	bool NeedToRotate;
public:	
	// Sets default values for this component's properties
	UWallMechanicalComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float DistanceFromWallToJump = 50.0f;

	UPROPERTY(EditAnywhere)
	float SpeedRotation = 50.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool WallJump();

	UFUNCTION(BlueprintCallable)
	void LaunchForWallJump();

	UFUNCTION(BlueprintCallable)
	void ClearWallToIgnore();

	UPROPERTY(BlueprintReadOnly)
	bool IsWallJumping;
		
	UPROPERTY(BlueprintReadWrite)
	bool HasToLaunchCharacter;
};
