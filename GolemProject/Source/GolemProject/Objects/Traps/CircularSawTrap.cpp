// Golem Project - Créajeux 2020


#include "CircularSawTrap.h"
#include "Components/StaticMeshComponent.h"
#include "GolemProjectCharacter.h"

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
	if (mesh)
	{
		mesh->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACircularSawTrap::OverlapActivation);
	}
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
				if (changeRotationOnChangeDirection)
				{
					rotationSpeed *= -1.f;
				}
			}
			FVector velocity = directionToNextCheckpoint.GetSafeNormal() * dist;
			SetActorLocation(GetActorLocation() + velocity);
			distanceToGo -= dist;
		}
		mesh->AddLocalRotation(FRotator::MakeFromEuler(FVector::RightVector * rotationSpeed * DeltaTime));
	}
}

void ACircularSawTrap::OverlapActivation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent != nullptr && OtherActor != nullptr && OtherComp != nullptr && (isActivate))
	{
		if (AGolemProjectCharacter* character = Cast<AGolemProjectCharacter>(OtherActor))
		{
			character->InflictDamage(damage);
		}
	}
}

