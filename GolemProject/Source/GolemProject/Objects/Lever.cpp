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
	MeshComponent = FindComponentByClass<UStaticMeshComponent>();
	CanBeActivated = true;
}

void ALever::ResetActivation()
{
	FVector4 color(1.0f, 1.0f, 1.0f, 1.0f);
	MeshComponent->SetVectorParameterValueOnMaterials("Color", color);
	CanBeActivated = true;
}

const bool ALever::Interact_Implementation(AActor* caller)
{
	bool haveActivate = false;
	HelperLibrary::Print(FString::Printf(TEXT("Interact with %s"), *(GetName())), 5.f, FColor::Green);
	if (CanBeActivated)
	{
		for (auto& objectToActivate : objectsToActivate)
		{
			if (objectToActivate != nullptr)
			{
				if (IActivable* activable = Cast<IActivable>(objectToActivate))
				{
					if (activationType == EActivationType::Activate)
					{
						activable->Execute_Activate(objectToActivate, this);
						if (UWorld* world = GetWorld())
						{
							FVector4 color(0.0f, 1.0f, 0.0f, 1.0f);
							MeshComponent->SetVectorParameterValueOnMaterials("Color", color);
						}
						Event_Interaction();
						haveActivate = true;
						CanBeActivated = false;
					}
					else if (activationType == EActivationType::Desactivate)
					{
						activable->Execute_Desactivate(objectToActivate, this);
						if (UWorld* world = GetWorld())
						{
							FVector4 color(1.0f, 0.0f, 0.0f, 1.0f);
							MeshComponent->SetVectorParameterValueOnMaterials("Color", color);
						}
						Event_Interaction();
						haveActivate = true;
						CanBeActivated = false;
					}
					else if (activationType == EActivationType::Switch)
					{
						activable->Execute_Switch(objectToActivate, this);
						/*if (UWorld* world = GetWorld())
						{
							FVector4 color(0.0f, 1.0f, 0.0f, 1.0f);
							MeshComponent->SetVectorParameterValueOnMaterials("Color", color);
							FTimerHandle timerHandle;
							world->GetTimerManager().SetTimer(timerHandle, this, &ALever::ResetActivation, 1.5f);
						}*/
						Event_Interaction();
						haveActivate = true;
						CanBeActivated = false;
					}
				}
				else
				{
					HelperLibrary::Print(FString::Printf(TEXT("%s don\'t implement Activable interface"), *(objectToActivate->GetName())), 5.f, FColor::Yellow);
				}
			}
		}
	}
	return haveActivate;
}