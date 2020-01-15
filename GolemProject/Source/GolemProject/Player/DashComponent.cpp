// Fill out your copyright notice in the Description page of Project Settings.


#include "DashComponent.h"
#include <Engine/Engine.h>
#include "GolemProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	//comm
	Super::BeginPlay();
	m_character = Cast<AGolemProjectCharacter>(GetOwner());
	m_canDash = true;
	if (m_character != nullptr)
	{
		if (UCharacterMovementComponent* cmc = m_character->GetCharacterMovement())
		{
			m_groundFriction = m_character->GetCharacterMovement()->GroundFriction;
		}
	}
}

void UDashComponent::StopDash()
{
	if (m_character != nullptr)
	{
		m_character->GetMovementComponent()->StopMovementImmediately();
		m_character->GetCharacterMovement()->GroundFriction = m_groundFriction;
	}
}

void UDashComponent::CanRedashDash()
{
	m_canDash = true;
}

// Called every frame
void UDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UDashComponent::Dash(FVector _direction)
{
	if (m_canDash && m_character != nullptr)
	{
		if (UCharacterMovementComponent* cmc = m_character->GetCharacterMovement())
		{
			cmc->GroundFriction = 0.0f;
			m_character->LaunchCharacter(_direction * m_forceDash, false, false);
			m_canDash = false;
			if (UWorld* world = GetWorld())
			{
				world->GetTimerManager().SetTimer(m_loopTimer, this, &UDashComponent::StopDash, m_timerStopDash, false);
				world->GetTimerManager().SetTimer(m_timerDash, this, &UDashComponent::CanRedashDash, m_cdDash, false);
			}
		}
	}
}

