// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"
#include "Engine/Engine.h"

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

}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ALever::Interact_Implementation(const AActor * caller) const
{
	bool haveActivate = false;
	for (auto& objectToActivate : objectsToActivate)
	{
		if (IActivable* activable = Cast<IActivable>(objectToActivate))
		{
			if (activationType == EActivationType::Activate)
			{
				activable->Activate(this);
				haveActivate = true;
			}
			else if (activationType == EActivationType::Desactivate)
			{
				activable->Desactivate(this);
				haveActivate = true;
			}
			else if (activationType == EActivationType::Switch)
			{
				activable->Switch(this);
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

