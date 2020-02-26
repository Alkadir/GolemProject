// Fill out your copyright notice in the Description page of Project Settings.


#include "SwingPhysics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GolemProjectCharacter.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Classes/Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "Player/GrappleComponent.h"
#include "CableComponent.h"
#include "Helpers/HelperLibrary.h"


SwingPhysics::SwingPhysics()
{
}

SwingPhysics::~SwingPhysics()
{
}

SwingPhysics::SwingPhysics(UGrappleComponent* _grappleHook)
{
	UWorld* world = _grappleHook->GetWorld();
	character = _grappleHook->GetCharacter();
	characterMovement = character->GetCharacterMovement();
	target = _grappleHook->GetClosestGrapplingHook();
	FVector vec = (target->GetActorLocation() - character->GetActorLocation());
	
	float dist = vec.Size();
	vec /= dist;
	dist *= 0.5f;

	if (world)
	{
		FActorSpawnParameters params;
		params.Name = "ActorCable";
		params.Instigator = character;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		/*AActor* cableActor = world->SpawnActor<AActor>(AActor::StaticClass(), character->GetTransform(), params);

		if (cableActor)
		{*/
		//	HelperLibrary::Print(cableActor->GetName());
			UCableComponent* cableComponentActor = NewObject<UCableComponent>(character->GetRootComponent(),UCableComponent::StaticClass(),TEXT("cableComp"));
			
			if (cableComponentActor)
			{
				HelperLibrary::Print(cableComponentActor->GetName());
				character->AddInstanceComponent(cableComponentActor);
				cableComponentActor->OnComponentCreated();
				cableComponentActor->EndLocation = target->GetActorLocation();
				cableComponentActor->RegisterComponent();
			}	
		//}
	}
}

void SwingPhysics::Tick(float _deltaTime)
{
}
