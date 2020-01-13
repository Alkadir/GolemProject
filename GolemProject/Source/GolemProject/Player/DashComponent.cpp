// Fill out your copyright notice in the Description page of Project Settings.


#include "DashComponent.h"
#include <Engine/Engine.h>
#include "GolemProjectCharacter.h"

// Sets default values for this component's properties
UDashComponent::UDashComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDashComponent::BeginPlay()
{
	Super::BeginPlay();	
	m_character = Cast<AGolemProjectCharacter>(GetOwner());
	// ...
	
}




// Called every frame
void UDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UDashComponent::Dash()
{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));
	if (m_character != nullptr)
	{
		m_character->LaunchCharacter(velocityDash, true, true);
	}
}

