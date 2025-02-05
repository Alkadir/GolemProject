// Golem Project - Créajeux 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rope.generated.h"

UCLASS()
class GOLEMPROJECT_API ARope : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	class USceneComponent* sceneComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* staticMeshComp;
	class AActor* target;
	class UGrappleComponent* grappleComp;
	FVector direction;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ARope();

	void HideMesh();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE void SetGrappleComponent(class UGrappleComponent* _grappleComp) { grappleComp = _grappleComp; };
	FORCEINLINE class AActor* GetTarget() { return target; };
	FORCEINLINE FVector GetDirection() { return direction; };
};
