// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableBloc.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "GolemProjectCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "GolemProjectCharacter.h"
#include "Helpers/HelperLibrary.h"


APushableBloc::APushableBloc()
{
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	if (mesh != nullptr && root != nullptr)
	{
		mesh->SetupAttachment(root);
		mesh->SetSimulatePhysics(false);
	}

}

const bool APushableBloc::Interact_Implementation(AActor* caller)
{
	if (pushingDirection.IsNearlyZero(0.01f) && pushingPosition.IsNearlyZero(0.01f) && pushingRotation.IsNearlyZero(0.01f) && !isUsed)
	{
		return false;
	}
	playerActor = Cast<AGolemProjectCharacter>(caller);
	if (playerActor == nullptr)
	{
		return false;
	}
	else
	{
		isUsed = !isUsed;
		if (isUsed)
		{
			playerActor->PushBloc(pushingDirection, pushingPosition, pushingRotation);
		}
		else
		{
			playerActor->StopPushBloc();
		}
		return true;
	}

	return false;
}