// Fill out your copyright notice in the Description page of Project Settings.


#include "DashComponent.h"
#include <Engine/Engine.h>
#include "GolemProjectCharacter.h"
#include "Helpers/HelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UDashComponent::UDashComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UDashComponent::BeginPlay()
{
	//comm
	Super::BeginPlay();
	SetTickGroup(ETickingGroup::TG_PostPhysics);
	HasDashInAir = false;
	m_character = Cast<AGolemProjectCharacter>(GetOwner());
	m_canDash = true;
	if (m_character != nullptr)
	{
		CharacterMovementCmpt = m_character->GetCharacterMovement();
	}
}

void UDashComponent::StopDash()
{
	if (m_character != nullptr && CharacterMovementCmpt != nullptr)
	{
		CharacterMovementCmpt->StopMovementImmediately();
		if (CurrentVelocity.IsZero())
		{
			CharacterMovementCmpt->AddImpulse(CurrentDirection * ForceAfterDash, true);
		}
		else
		{
			CharacterMovementCmpt->AddImpulse(CurrentVelocity, true);
		}
		m_character->ResetFriction();
		isDashing = false;
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

	if (isDashing && m_character && DashTrailClass)
	{
		GetWorld()->SpawnActor<AActor>(DashTrailClass, m_character->GetMesh()->GetComponentTransform());
	}
}

void UDashComponent::Dash(FVector _direction)
{
	if (CharacterMovementCmpt != nullptr && m_character != nullptr)
	{
		if (m_canDash && !HasDashInAir)
		{
			float dashForce = ForceDash;
			if (CharacterMovementCmpt->IsFalling())
			{
				HasDashInAir = true;
				dashForce = ForceDashInTheAir;
			}
			CharacterMovementCmpt->GroundFriction = 0.0f;
			CurrentDirection = _direction;
			CurrentDirection.Z = 0.0f;
			CurrentVelocity = m_character->GetVelocity().Size() * _direction;
			m_character->LaunchCharacter(_direction * dashForce, false, false);
			isDashing = true;
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

void UDashComponent::CancelDash()
{
	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().ClearTimer(m_loopTimer);
	}
	if (m_character != nullptr && CharacterMovementCmpt != nullptr)
	{
		CharacterMovementCmpt->StopMovementImmediately();
		m_character->ResetFriction();
		isDashing = false;
	}
}

void UDashComponent::CancelDashAndResetCD()
{
	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().ClearTimer(m_loopTimer);
		world->GetTimerManager().ClearTimer(m_timerDash);
	}
	if (m_character != nullptr && CharacterMovementCmpt != nullptr)
	{
		CharacterMovementCmpt->StopMovementImmediately();
		m_character->ResetFriction();
		isDashing = false;
		m_canDash = true;
		HasDashInAir = false;
	}
}

