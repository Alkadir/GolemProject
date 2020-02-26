// Fill out your copyright notice in the Description page of Project Settings.


//#include "PushableBloc.h"
//#include "Components/SceneComponent.h"
//#include "Components/BoxComponent.h"
//#include "GolemProjectCharacter.h"
//#include "Components/StaticMeshComponent.h"
//#include "GolemProjectCharacter.h"
//
//APushableBloc::APushableBloc()
//{
//	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
//	SetRootComponent(root);
//
//	boxCollider = CreateDefaultSubobject<UBoxComponent>(FName("Box collider"));
//	boxCollider->SetupAttachment(GetRootComponent());
//
//	boxCollider->SetBoxExtent(colliderSize);
//	boxCollider->SetRelativeLocation(boxOffset);
//}
//
//const bool APushableBloc::Interact_Implementation(AActor* caller)
//{
//	playerActor = Cast<AGolemProjectCharacter>(caller);
//	if (playerActor == nullptr)
//	{
//		return false;
//	}
//	else
//	{
//		playerActor->PushBloc();
//
//		isUsed = !isUsed;
//
//		UStaticMeshComponent* mesh = this->FindComponentByClass<UStaticMeshComponent>();
//
//		//Attach to player and don't collide with pawn anymore
//		if (isUsed)
//		{
//			if (mesh != nullptr)
//			{
//				mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);
//			}
//
//			AttachToActor(playerActor, FAttachmentTransformRules::KeepWorldTransform);
//		}
//		//Detach from player and block pawn again
//		else
//		{
//			if (mesh != nullptr)
//			{
//				mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Block);
//			}
//
//			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//		}
//
//		return true;
//	}
//
//	return false;
//}
//
//
//void APushableBloc::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
//{
//	boxCollider->SetBoxExtent(colliderSize);
//	boxCollider->SetRelativeLocation(boxOffset);
//
//	Super::PostEditChangeProperty(PropertyChangedEvent);
//}
