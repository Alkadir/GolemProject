// Golem Project - Créajeux 2020


#include "ObjectActivator.h"
#include "Helpers/HelperLibrary.h"

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
	if (objectsToActivate.Num() == 0)
	{
		HelperLibrary::Print(FString::Printf(TEXT("WARNING %s don\'t have objects to activate"), *(GetName())), 5.f, FColor::Yellow);
	}
	for (auto& objectToActivate : objectsToActivate)
	{
		if (objectToActivate == nullptr)
		{
			HelperLibrary::Print(FString::Printf(TEXT("WARNING %s have none value on object to activate"), *(GetName())), 5.f, FColor::Red);
		}
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
			for (auto& objectToActivate : objectsToActivate)
			{
				if (objectToActivate != nullptr)
				{
					activable = Cast<IActivable>(objectToActivate);
					if (activable != nullptr)
					{
						activable->Execute_Desactivate(objectToActivate, this);
					}
					else
					{
						HelperLibrary::Print( FString::Printf(TEXT("%s don\'t implement Activable interface"), *(objectToActivate->GetName())), 5.f, FColor::Red);
					}
				}
			}
			return false;
		}
	}

	for (auto& objectToActivate : objectsToActivate)
	{
		if (objectToActivate != nullptr)
		{
			activable = Cast<IActivable>(objectToActivate);
			if (activable != nullptr)
			{
				activable->Execute_Activate(objectToActivate, this);
			}
			else
			{
				HelperLibrary::Print(FString::Printf(TEXT("%s don\'t implement Activable interface"), *(objectToActivate->GetName())), 5.f, FColor::Red);
			}
		}
	}
	return true;
}

const bool AObjectActivator::HaveErrorOnCaller(AActor* caller)
{
	if (caller == nullptr)
	{
		HelperLibrary::Print(TEXT("caller is null"), 5.f, FColor::Red);
		return true;
	}
	if (!statesObjects.Contains(caller))
	{
		HelperLibrary::Print(FString::Printf(TEXT("%s don\'t have reference on %s. You forgot to add it to activators array"), *(GetName()), *(caller->GetName())), 5.f, FColor::Red);
		return true;
	}
	return false;
}

const bool AObjectActivator::Activate_Implementation(AActor* caller)
{
	if (HaveErrorOnCaller(caller))
	{
		return false;
	}
	statesObjects[caller] = true;
	IsActivated();
	return true;
}

const bool AObjectActivator::Desactivate_Implementation(AActor* caller)
{
	if (HaveErrorOnCaller(caller))
	{
		return false;
	}
	statesObjects[caller] = false;
	IsActivated();
	return true;
}

const bool AObjectActivator::Switch_Implementation(AActor* caller)
{
	if (HaveErrorOnCaller(caller))
	{
		return false;
	}
	statesObjects[caller] = !statesObjects[caller];
	IsActivated();
	return true;
}

