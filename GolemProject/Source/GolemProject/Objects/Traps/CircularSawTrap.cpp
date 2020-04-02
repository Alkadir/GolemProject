// Golem Project - Créajeux 2020


#include "CircularSawTrap.h"
#include "Components/StaticMeshComponent.h"

ACircularSawTrap::ACircularSawTrap()
{
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);
	if (root)
	{
		destination = CreateDefaultSubobject<USceneComponent>(TEXT("DestinationDoor"));
		if (destination)
		{
			destination->SetupAttachment(root);
		}
		mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
		if (mesh)
		{
			mesh->SetupAttachment(root);
		}
	}

}

void ACircularSawTrap::BeginPlay()
{
	Super::BeginPlay();
	startPos = GetActorLocation();
	if (destination)
	{
		destinationPos = destination->GetComponentLocation();
	}
	FVector toDestination = destinationPos - startPos;
	FRotator rotation = toDestination.ToOrientationRotator();
	SetActorRotation(rotation);
}

void ACircularSawTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isActivate)
	{
		float distanceToGo = speed * DeltaTime;
		while (distanceToGo > 0)
		{
			FVector directionToNextCheckpoint;
			if (dir == 1)
			{
				directionToNextCheckpoint = destinationPos - GetActorLocation();
			}
			else
			{
				directionToNextCheckpoint = startPos - GetActorLocation();
			}
			float dist = distanceToGo;
			if (directionToNextCheckpoint.SizeSquared() < dist * dist)
			{
				dist = directionToNextCheckpoint.Size();
				dir = -dir;
			}
			FVector velocity = directionToNextCheckpoint.GetSafeNormal() * dist;
			SetActorLocation(GetActorLocation() + velocity);
			distanceToGo -= dist;
		}
		mesh->AddLocalRotation(FRotator::MakeFromEuler(FVector::RightVector * rotationSpeed * DeltaTime));
	}
}
