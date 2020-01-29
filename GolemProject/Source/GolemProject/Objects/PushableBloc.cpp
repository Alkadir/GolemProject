// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableBloc.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "GolemProjectCharacter.h"
#include "Components/StaticMeshComponent.h"

APushableBloc::APushableBloc()
{
	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	boxCollider = CreateDefaultSubobject<UBoxComponent>(FName("Box collider"));
	boxCollider->SetupAttachment(GetRootComponent());

}

void APushableBloc::BeginPlay()
{
	boxCollider->SetBoxExtent(FVector::OneVector * colliderSize);
	//boxCollider->SetRelativeLocation(FVector(0.0f, 0.0f, colliderSize / 2.0f));

	boxCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &APushableBloc::OnOverlapBegin);
	boxCollider->OnComponentEndOverlap.AddUniqueDynamic(this, &APushableBloc::OnOverlapEnd);
}

const bool APushableBloc::Interact_Implementation(const AActor* caller)
{
	if (playerActor != nullptr)
	{
		playerActor->PushBloc();

		isUsed = !isUsed;

		UStaticMeshComponent* mesh = this->FindComponentByClass<UStaticMeshComponent>();

		//Attach to player and don't collide with pawn anymore
		if (isUsed)
		{
			if (mesh != nullptr)
			{
				mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);
			}

			AttachToActor(playerActor, FAttachmentTransformRules::KeepWorldTransform);
		}
		//Detach from player and block pawn again
		else
		{
			if (mesh != nullptr)
			{
				mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Block);
			}
			
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}

		return true;
	}

	return false;
}

void APushableBloc::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGolemProjectCharacter* player = Cast<AGolemProjectCharacter>(OtherActor);
	if (player != nullptr)
	{
		playerActor = player;

		if (!isUsed)
		{
			playerActor->SetInteractable(this);
		}
	}
}

void APushableBloc::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AGolemProjectCharacter* player = Cast<AGolemProjectCharacter>(OtherActor);
	if (player != nullptr)
	{
		if (!isUsed && playerActor != nullptr)
		{
			playerActor->SetInteractable(nullptr);
		}

		playerActor = nullptr;
	}
}
