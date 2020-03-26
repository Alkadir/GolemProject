// Golem Project - Créajeux 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RaycastingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOLEMPROJECT_API URaycastingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URaycastingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	class USkeletalMeshComponent* MeshComponent;

	class AGolemProjectCharacter* Character;

	class UHealthComponent* HealthComponent;

	float LineForIK(FName _boneName);

	UPROPERTY(EditAnywhere)
	FName LeftFootName;

	UPROPERTY(EditAnywhere)
	FName RightFootName;

	UPROPERTY(EditAnywhere)
	float MaxDistanceBelowCapsule;

	UPROPERTY(EditAnywhere)
	float InterpSpeed;

	FVector BoneLocation;

	UPROPERTY(BlueprintReadOnly)
	float LeftFootOffset;
	UPROPERTY(BlueprintReadOnly)
	float RightFootOffset;
	UPROPERTY(BlueprintReadOnly)
	float PelvisOffset;

	int32 indexFootR;
	int32 indexFootL;
	bool FootRGrounded;
	bool FootLGrounded;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
