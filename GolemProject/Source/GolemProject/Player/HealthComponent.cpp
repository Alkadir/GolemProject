// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "GolemProjectCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	if (AActor* owner = GetOwner())
	{
		Player = Cast<AGolemProjectCharacter>(owner);
	}
	// ...
	
}


void UHealthComponent::InflictDamage(int _damage)
{
	Life -= _damage;
	CanTakeDamage = false;
	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().SetTimer(TimerHandlerInvul, this, &UHealthComponent::ResetInvulnerability, TimerInvulnerability, true);
	}
	if (Life <= 0)
	{
		Life = 0;
		//kill player
	}
}

void UHealthComponent::ResetInvulnerability()
{
	CanTakeDamage = true;
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

