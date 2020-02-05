// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"

#include "Engine/Engine.h"
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
}

void APressurePlate::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("End overlap"));
}

void APressurePlate::BeginPlay()
{
	boxCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &APressurePlate::OnOverlapBegin);
	boxCollider->OnComponentEndOverlap.AddUniqueDynamic(this, &APressurePlate::OnOverlapEnd);
}

void APressurePlate::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	boxCollider->SetBoxExtent(colliderSize);
	boxCollider->SetRelativeLocation(boxOffset);

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
