// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"

#include "Engine/Engine.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Math/UnrealMathVectorCommon.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

APressurePlate::APressurePlate()
{
	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	boxCollider->SetupAttachment(root);
}

void APressurePlate::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Begin overlap"));

	UWorld* world = GetWorld();
	if (world)
	{
		if (!isPressed)
		{
			world->GetTimerManager().SetTimer(pressedTimerHandle, this, &APressurePlate::OnPressed, delay);
		}

		world->GetTimerManager().ClearTimer(releasedTimerHandle);
	}
}

void APressurePlate::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("End overlap"));

	UWorld* world = GetWorld();
	if (world)
	{
		if (isPressed)
		{
			world->GetTimerManager().SetTimer(releasedTimerHandle, this, &APressurePlate::OnReleased, delay);
		}

		world->GetTimerManager().ClearTimer(pressedTimerHandle);
	}
}

void APressurePlate::OnPressed()
{
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Yellow, TEXT("Pressed!"));

	if (!isPressed)
	{
		FVector newPosition;

		newPosition.Z = GetActorLocation().Z - 4.f;
		SetActorLocation(newPosition);
	}

	isPressed = true;
}

void APressurePlate::OnReleased()
{
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Yellow, TEXT("Released!"));
	if (isPressed)
	{
		FVector newPosition;
		newPosition.Z = GetActorLocation().Z + 4.f;
		SetActorLocation(newPosition);
	}
	isPressed = false;
}

void APressurePlate::BeginPlay()
{
	boxCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &APressurePlate::OnOverlapBegin);
	boxCollider->OnComponentEndOverlap.AddUniqueDynamic(this, &APressurePlate::OnOverlapEnd);
}

#if WITH_EDITOR
void APressurePlate::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	boxCollider->SetBoxExtent(colliderSize);
	boxCollider->SetRelativeLocation(boxOffset);

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
