// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"
#include "Engine/Engine.h"
#include "Components/StaticMeshComponent.h"
#include "GolemProjectCharacter.h"

// Sets default values
ALever::ALever()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{
	Super::BeginPlay();

	collider = FindComponentByClass<UStaticMeshComponent>();

	collider->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALever::OnOverlapBegin);
	collider->OnComponentEndOverlap.AddUniqueDynamic(this, &ALever::OnOverlapEnd);
}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

const bool ALever::Interact_Implementation(AActor* caller)
{
	bool haveActivate = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Interact")));
	for (auto& objectToActivate : objectsToActivate)
	{
		if (IActivable * activable = Cast<IActivable>(objectToActivate))
		{
			if (activationType == EActivationType::Activate)
			{
				activable->Execute_Activate(objectToActivate, this);
				haveActivate = true;
			}
			else if (activationType == EActivationType::Desactivate)
			{
				activable->Execute_Desactivate(objectToActivate, this);
				haveActivate = true;
			}
			else if (activationType == EActivationType::Switch)
			{
				activable->Execute_Switch(objectToActivate, this);
				haveActivate = true;
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s don\'t implement Activable interface"), *(objectToActivate->GetName())));
		}
	}
	return haveActivate;
}

void ALever::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGolemProjectCharacter* player = Cast<AGolemProjectCharacter>(OtherActor);
	if (player != nullptr)
	{
		player->SetInteractable(this);
	}
}

void ALever::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AGolemProjectCharacter* player = Cast<AGolemProjectCharacter>(OtherActor);
	if (player != nullptr)
	{
		player->SetInteractable(nullptr);
	}
}