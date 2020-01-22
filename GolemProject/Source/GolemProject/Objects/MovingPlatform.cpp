// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Helpers/HelperLibrary.h"

// Sets default values
AMovingPlatform::AMovingPlatform(const FObjectInitializer& OI)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* Root = OI.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	SetRootComponent(Root);
	pathParent = CreateDefaultSubobject<USceneComponent>(TEXT("pathParen"));
	pathParent->SetupAttachment(Root);
	path1 = CreateDefaultSubobject<USceneComponent>(TEXT("Path1"));
	path1->SetupAttachment(pathParent);

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (speeds.Num() < worldCheckpoint.Num())
	{
		HelperLibrary::Print("ERROR Speed is not initialized for all path points", 15.f, FColor::Red);
		return;
	}
	if (waitTimes.Num() < worldCheckpoint.Num())
	{
		HelperLibrary::Print("ERROR WaitTime is not initialized for all path points", 15.f, FColor::Red);
		return;
	}

	if (worldCheckpoint.Num() <= 1 || !isActivate || direction == EMovingDirection::None || isPause)
	{
		return;
	}
	if (waitTime > 0)
	{
		waitTime -= DeltaTime;
		return;
	}

	int refIndex = direction == EMovingDirection::Forward ? currentIndex : nextIndex;
	MoveLine(DeltaTime);
	//if (movingType[refIndex] == MovingType.line)
	//{
	//	MoveLine();
	//}
	//else if (movingType[refIndex] == MovingType.curve)
	//{
	//	MoveCurve();
	//}
}

void AMovingPlatform::Init()
{
	TArray<USceneComponent*> childrens;
	pathParent->GetChildrenComponents(false, childrens);
	if (childrens.Num() == 0)
	{
		HelperLibrary::Print("WARNING There is no path", 15.f, FColor::Yellow);
	}
	for (auto& path : childrens)
	{
		worldCheckpoint.Add(path->GetComponentLocation());
	}
	if (waitTimes.Num() > 0)
	{
		waitTime = waitTimes[0];
	}
	else
	{
		waitTime = 0.f;
	}
	if (startIndexCheckpoint >= 0 && startIndexCheckpoint < worldCheckpoint.Num())
	{
		currentIndex = startIndexCheckpoint;
	}
	else
	{
		currentIndex = 0;
	}
	if (direction == EMovingDirection::Backward)
	{
		dir = -1;
	}
	else
	{
		dir = 1;
	}
	nextIndex = currentIndex + dir;
	if (nextIndex < 0)
	{
		nextIndex = worldCheckpoint.Num() - 1;
	}
	else if (nextIndex >= worldCheckpoint.Num())
	{
		nextIndex = 0;
	}
	velocity = FVector::ZeroVector;
	SetActorLocation(worldCheckpoint[currentIndex]);
	isPause = false;
	//parents = new Dictionary<Collider, Transform>();
}

void AMovingPlatform::MoveLine(float dt)
{
	FVector startPos = worldCheckpoint[direction == EMovingDirection::Forward ? currentIndex : nextIndex];
	FVector endPos = worldCheckpoint[direction == EMovingDirection::Forward ? nextIndex : currentIndex];
	float totalDistance = (endPos - startPos).Size();
	float traveledDistance = (GetActorLocation() - startPos).Size();
	float percentageTraveledDistance = traveledDistance / totalDistance;
	float distanceToGo = (direction == EMovingDirection::Forward ? speeds[currentIndex] : speeds[nextIndex]) * dt;/* *
		(direction == EMovingDirection::Forward ? speedCurve[currentIndex].Evaluate(percentageTraveledDistance) : speedCurve[nextIndex].Evaluate(percentageTraveledDistance));*/
	while (distanceToGo > 0 && waitTime <= 0.f)
	{
		FVector direction = worldCheckpoint[nextIndex] - GetActorLocation();
		float dist = distanceToGo;
		if (direction.SizeSquared() < dist * dist)
		{
			if (platformType != EMovingPlatformType::Once)
			{
				dist = direction.Size();
			}
			SetNextIndex();
		}
		velocity = direction.GetSafeNormal() * dist;
		SetActorLocation(GetActorLocation() + velocity);
		distanceToGo -= dist;
	}
}

void AMovingPlatform::SetNextIndex()
{
	currentIndex = nextIndex;
	waitTime = waitTimes[currentIndex];
	if (dir > 0)
	{
		nextIndex += 1;
		if (nextIndex >= worldCheckpoint.Num())
		{ //we reach the end

			switch (platformType)
			{
			case EMovingPlatformType::PingPong:
				nextIndex = worldCheckpoint.Num() - 2;
				dir = -1;
				direction = EMovingDirection::Backward;
				break;
			case EMovingPlatformType::Loop:
				nextIndex = 0;
				break;
			case EMovingPlatformType::OnceLoop:
				nextIndex = 0;
				break;
			case EMovingPlatformType::Once:
				nextIndex = worldCheckpoint.Num() - 2;
				direction = EMovingDirection::Backward;
				dir = -1;
				Desactivate(this);
				break;
			}
		}
		else if (nextIndex == 1 && platformType == EMovingPlatformType::OnceLoop)
		{
			Desactivate(this);
		}
	}
	else
	{
		nextIndex -= 1;
		if (nextIndex < 0)
		{ //reached the beginning

			switch (platformType)
			{
			case EMovingPlatformType::PingPong:
				nextIndex = 1;
				dir = 1;
				direction = EMovingDirection::Forward;
				break;
			case EMovingPlatformType::Loop:
				nextIndex = worldCheckpoint.Num() - 1;
				break;
			case EMovingPlatformType::OnceLoop:
				nextIndex = worldCheckpoint.Num() - 1;
				Desactivate(this);
				break;
			case EMovingPlatformType::Once:
				nextIndex = 1;
				direction = EMovingDirection::Forward;
				dir = 1;
				Desactivate(this);
				break;
			}
		}
	}
}

const bool AMovingPlatform::Activate_Implementation(const AActor* caller)
{
	if (isActivate)
	{
		return false;
	}
	isActivate = true;
	return true;
}

const bool AMovingPlatform::Desactivate_Implementation(const AActor* caller)
{
	if (!isActivate)
	{
		return false;
	}
	isActivate = false;
	return true;
}

const bool AMovingPlatform::Switch_Implementation(const AActor* caller)
{
	isActivate = !isActivate;
	return true;
}

