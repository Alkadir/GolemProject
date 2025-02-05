// Golem Project - Créajeux 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillItemActivator.generated.h"

UCLASS()
class GOLEMPROJECT_API ASkillItemActivator : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* staticMeshComp;
	
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	bool fist = false;

	UPROPERTY(EditAnywhere)
	bool grapple;
public:	
	// Sets default values for this actor's properties
	ASkillItemActivator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
