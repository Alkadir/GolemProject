// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "GolemProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Helpers/HelperLibrary.h"
#include "GameFramework/PlayerController.h"

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
		PlayerController = Cast<APlayerController>(Player->GetController());
	}
	CanTakeDamage = true;
}


void UHealthComponent::InflictDamage(int _damage)
{
	if (CanTakeDamage)
	{
		Life -= _damage;
		CanTakeDamage = false;
		if (Life <= 0)
		{
			Life = 0;
			if (UWorld* world = GetWorld())
			{
				world->GetTimerManager().SetTimer(TimerHandlerRespawn, this, &UHealthComponent::Respawn, TimerRespawn, false);
			}
			if (PlayerController != nullptr && Player != nullptr)
			{
				Player->DisableInput(PlayerController);
			}
			/*if (UCharacterMovementComponent* cmc = Player->GetCharacterMovement())
			{
				cmc->Deactivate();

			}*/
		}
		else
		{
			if (UWorld* world = GetWorld())
			{
				world->GetTimerManager().SetTimer(TimerHandlerInvul, this, &UHealthComponent::ResetInvulnerability, TimerInvulnerability, false);
			}
		}
	}
}

void UHealthComponent::ResetInvulnerability()
{
	CanTakeDamage = true;
}

void UHealthComponent::Respawn()
{
	Life = MaxLife;
	CanTakeDamage = true;
	Player->SetActorLocation(LastPositionGrounded);
	if (PlayerController != nullptr && Player != nullptr)
	{
		Player->EnableInput(PlayerController);
	}
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::SetLastPositionGrounded(FVector _lastPositionGrounded)
{
	LastPositionGrounded = _lastPositionGrounded;
}

void UHealthComponent::SetPositionCheckPoint(FVector _positionCheckPoint)
{
	PositionCheckPoint = _positionCheckPoint;
}

