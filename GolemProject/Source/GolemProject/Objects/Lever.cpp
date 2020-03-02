// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"
#include "Helpers/HelperLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "GolemProjectCharacter.h"

// Sets default values
ALever::ALever()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ALever::BeginPlay()
{
	Super::BeginPlay();
	if (objectsToActivate.Num() == 0)
	{
		HelperLibrary::Print(FString::Printf(TEXT("WARNING %s don\'t have objects to interact"), *(GetName())), 5.f, FColor::Yellow);
	}
	for (auto& objectToActivate : objectsToActivate)
	{
		if (objectToActivate == nullptr)
		{
			HelperLibrary::Print(FString::Printf(TEXT("WARNING %s have none value on object to activate"), *(GetName())), 5.f, FColor::Yellow);
		}
	}
}

const bool ALever::Interact_Implementation(AActor* caller)
{
	bool haveActivate = false;
	HelperLibrary::Print(FString::Printf(TEXT("Interact with %s"), *(GetName())), 5.f, FColor::Green);
	for (auto& objectToActivate : objectsToActivate)
	{
		if (objectToActivate != nullptr)
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
				HelperLibrary::Print(FString::Printf(TEXT("%s don\'t implement Activable interface"), *(objectToActivate->GetName())), 5.f, FColor::Yellow);
			}
		}
	}
	return haveActivate;
}