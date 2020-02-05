// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Helpers/HelperLibrary.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"

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

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void AMovingPlatform::Init()
{
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
	if (alwaysActive)
	{
		isActivate = true;
		if (isStair)
		{
			platformType = EMovingPlatformType::PingPong;
		}
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
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (worldCheckpoint.Num() <= 1 || !isActivate || direction == EMovingDirection::None || isPause)
	{
		return;
	}
	if (waitTime > 0)
	{
		waitTime -= DeltaTime;
		return;
	}

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

	int refIndex = direction == EMovingDirection::Forward ? currentIndex : nextIndex;

	if (movingType.IsValidIndex(refIndex))
	{
		if (movingType[refIndex] == EMovingType::Line)
		{
			MoveLine(DeltaTime);
		}
		else if (movingType[refIndex] == EMovingType::Curve)
		{
			MoveCurve(DeltaTime, refIndex);
		}
	}
	else
	{
		MoveLine(DeltaTime);
	}
}

void AMovingPlatform::MoveLine(float dt)
{
	//FVector startPos = worldCheckpoint[direction == EMovingDirection::Forward ? currentIndex : nextIndex];
	//FVector endPos = worldCheckpoint[direction == EMovingDirection::Forward ? nextIndex : currentIndex];
	//float totalDistance = (endPos - startPos).Size();
	//float traveledDistance = (GetActorLocation() - startPos).Size();
	//float percentageTraveledDistance = traveledDistance / totalDistance;
	float distanceToGo = (direction == EMovingDirection::Forward ? speeds[currentIndex] : speeds[nextIndex]) * dt;/* *
		(direction == EMovingDirection::Forward ? speedCurve[currentIndex].Evaluate(percentageTraveledDistance) : speedCurve[nextIndex].Evaluate(percentageTraveledDistance));*/
	while (distanceToGo > 0 && waitTime <= 0.f)
	{
		FVector directionToNextCheckpoint = worldCheckpoint[nextIndex] - GetActorLocation();
		float dist = distanceToGo;
		if (directionToNextCheckpoint.SizeSquared() < dist * dist)
		{
			if (platformType != EMovingPlatformType::Once)
			{
				dist = directionToNextCheckpoint.Size();
			}
			SetNextIndex();
		}
		velocity = directionToNextCheckpoint.GetSafeNormal() * dist;
		SetActorLocation(GetActorLocation() + velocity);
		distanceToGo -= dist;
	}
}

void AMovingPlatform::MoveCurve(float dt, int refIndex)
{
	float distanceToGo = (direction == EMovingDirection::Forward ? speeds[currentIndex] : speeds[nextIndex]) * dt; /**
		(direction == EMovingDirection::Forward ? speedCurve[currentIndex].Evaluate(timerLerp) : speedCurve[nextIndex].Evaluate(1.f - timerLerp));*/
	while (distanceToGo > 0.f && waitTime <= 0.f)
	{
		if (USplineComponent * spline = Cast<USplineComponent>(childrens[refIndex]))
		{
			FVector nextPos = worldCheckpoint[refIndex] + spline->GetLocationAtTime(direction == EMovingDirection::Forward ? timerLerp : 1.f - timerLerp, ESplineCoordinateSpace::Local, true);

			FVector directionNextPos = nextPos - GetActorLocation();
			float dist = distanceToGo;
			if (directionNextPos.SizeSquared() < dist * dist)
			{
				dist = directionNextPos.Size();
				if (timerLerp >= 1.f)
				{
					SetNextIndex();
					timerLerp = 0.f;
				}
				else
				{
					timerLerp += 0.01f;
					if (timerLerp > 1.f)
					{
						timerLerp = 1.f;
					}
				}
			}
			velocity = directionNextPos.GetSafeNormal() * dist;
			SetActorLocation(GetActorLocation() + velocity);
			distanceToGo -= dist;
		}
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
				isActivate = false;
				break;
			}
		}
		else if (nextIndex == 1 && platformType == EMovingPlatformType::OnceLoop)
		{
			isActivate = false;
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
				isActivate = false;
				break;
			case EMovingPlatformType::Once:
				nextIndex = 1;
				direction = EMovingDirection::Forward;
				dir = 1;
				isActivate = false;
				break;
			}
		}
	}
}

const bool AMovingPlatform::Activate_Implementation(AActor* caller)
{
	if (isStair && direction != EMovingDirection::Forward)
	{
		direction = EMovingDirection::Forward;
		isActivate = true;
		SetNextIndex();
		return true;
	}
	if (isActivate)
	{
		return false;
	}
	isActivate = true;
	return true;
}

const bool AMovingPlatform::Desactivate_Implementation(AActor* caller)
{
	if (isStair && direction != EMovingDirection::Backward)
	{
		direction = EMovingDirection::Backward;
		SetNextIndex();
		isActivate = true;
		return true;
	}
	if (!isActivate)
	{
		return false;
	}
	else
	{
		isActivate = false;
	}
	return true;
}

const bool AMovingPlatform::Switch_Implementation(AActor* caller)
{
	if (isStair)
	{
		if (direction == EMovingDirection::Forward)
		{
			direction = EMovingDirection::Backward;
		}
		else if (direction == EMovingDirection::Backward)
		{
			direction = EMovingDirection::Forward;
		}
		SetNextIndex();
		isActivate = true;
	}
	else
	{
		isActivate = !isActivate;
	}
	return true;
}

