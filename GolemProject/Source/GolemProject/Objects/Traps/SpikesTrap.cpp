// Golem Project - Créajeux 2020


#include "SpikesTrap.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GolemProjectCharacter.h"
#include "Helpers/HelperLibrary.h"

ASpikesTrap::ASpikesTrap()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
	ArraySpikes = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Spike");
	ArraySpikes->SetupAttachment(StaticMesh);
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>("TriggerActivation");
	BoxTrigger->SetupAttachment(StaticMesh);
}

void ASpikesTrap::BeginPlay()
{
	Super::BeginPlay();
	BoxDmgTrigger = FindComponentByClass<UBoxComponent>();
	BoxDmgTrigger->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASpikesTrap::OverlapDamage);
	BoxTrigger->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASpikesTrap::OverlapActivation);
	BoxTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	ArraySpikes->SetRelativeLocation(FVector::ZeroVector);
	FTransform tr;
	tr.SetScale3D(FVector(0.1f, 0.1f, 1.0f));
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			//do not change those fucking values
			tr.SetLocation(FVector(j * 20.0f - 40.0f, i * 20.0f - 40.0f, -100.0f));
			ArraySpikes->AddInstance(tr);
		}
	}
	EndLocation = ArraySpikes->GetComponentLocation() + FVector(0.0f, 0.0f, 100.0f);
	if (alwaysActive)
	{
		Activate(nullptr);
	}
}

void ASpikesTrap::Tick(float _deltaTime)
{
	if (IsTriggered)
	{
		ArraySpikes->SetWorldLocation(FMath::Lerp(ArraySpikes->GetComponentLocation(), EndLocation, _deltaTime));
	}
}

void ASpikesTrap::OverlapDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent != nullptr && OtherActor != nullptr && OtherComp != nullptr && isActivate && IsTriggered)
	{
		if (AGolemProjectCharacter * character = Cast<AGolemProjectCharacter>(OtherActor))
		{

		}
	}
}

void ASpikesTrap::OverlapActivation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent != nullptr && OtherActor != nullptr && OtherComp != nullptr && isActivate)
	{
		HelperLibrary::Print("azrfef");
		if (AGolemProjectCharacter * character = Cast<AGolemProjectCharacter>(OtherActor))
		{
			HelperLibrary::Print("coucou");
			IsTriggered = true;
		}
	}
}
