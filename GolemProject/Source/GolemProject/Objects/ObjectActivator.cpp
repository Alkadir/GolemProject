// Golem Project - Créajeux 2020


#include "ObjectActivator.h"

// Sets default values
AObjectActivator::AObjectActivator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjectActivator::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < activators.Num(); i++)
	{
		statesObjects.Add(activators[i].actor, activators[i].baseState);
	}
}

// Called every frame
void AObjectActivator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AObjectActivator::IsActivated()
{
	IActivable* activable;
	for (int i = 0; i < activators.Num(); i++)
	{
		if (statesObjects[activators[i].actor] != activators[i].neededState)
		{
			for(auto& actor : objectsToActivate)
			{
				activable = Cast<IActivable>(actor);
				if (activable != nullptr)
				{
					activable->Execute_Desactivate(actor, this);
				}
			}
			return false;
		}
	}

	for (auto& actor : objectsToActivate)
	{
		activable = Cast<IActivable>(actor);
		if (activable != nullptr)
		{
			activable->Execute_Activate(actor, this);
		}
	}
	return true;
}

const bool AObjectActivator::Activate_Implementation(AActor* caller)
{
	statesObjects[caller] = true;
	IsActivated();
	return true;
}

const bool AObjectActivator::Desactivate_Implementation(AActor* caller)
{
	statesObjects[caller] = false;
	IsActivated();
	return true;
}

const bool AObjectActivator::Switch_Implementation(AActor* caller)
{
	statesObjects[caller] = !statesObjects[caller];
	IsActivated();
	return true;
}

