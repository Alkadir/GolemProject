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
	HasDashInAir = false;
	m_character = Cast<AGolemProjectCharacter>(GetOwner());
	m_canDash = true;
	if (m_character != nullptr)
	{
		CharacterMovementCmpt = m_character->GetCharacterMovement();
		if (CharacterMovementCmpt != nullptr)
		{
			m_groundFriction = CharacterMovementCmpt->GroundFriction;
		}
	}
}

void UDashComponent::StopDash()
{
	if (m_character != nullptr)
	{
		m_character->GetMovementComponent()->StopMovementImmediately();
		if (CharacterMovementCmpt != nullptr)
		{
			if (CurrentVelocity.IsZero())
			{
				CharacterMovementCmpt->AddImpulse(CurrentDirection * ForceAfterDash, true);
			}
			else
			{
				CharacterMovementCmpt->AddImpulse(CurrentVelocity, true);
			}
			CharacterMovementCmpt->GroundFriction = m_groundFriction;
		}
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
}

void UDashComponent::Dash(FVector _direction)
{
	if (CharacterMovementCmpt != nullptr)
	{
		if (m_canDash && m_character != nullptr && !HasDashInAir)
		{
			if (CharacterMovementCmpt->IsFalling())	HasDashInAir = true;
			CharacterMovementCmpt->GroundFriction = 0.0f;
			CurrentDirection = _direction;
			CurrentVelocity = m_character->GetVelocity().GetAbs() * _direction;
			CurrentVelocity.Z = 0.0f;
			m_character->LaunchCharacter(_direction * ForceDash, false, false);
			m_canDash = false;
			if (UWorld* world = GetWorld())
			{
				world->GetTimerManager().SetTimer(m_loopTimer, this, &UDashComponent::StopDash, TimerStopDash, false);
				world->GetTimerManager().SetTimer(m_timerDash, this, &UDashComponent::CanRedashDash, CDDash, false);
			}
		}
	}
}

void UDashComponent::ResetDashInAir()
{
	HasDashInAir = false;
}

