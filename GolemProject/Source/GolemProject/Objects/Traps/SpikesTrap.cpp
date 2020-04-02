// Golem Project - Créajeux 2020


#include "SpikesTrap.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "GolemProjectCharacter.h"
#include "Helpers/HelperLibrary.h"

ASpikesTrap::ASpikesTrap()
{
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
	RootComponent = RootComp;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	if (StaticMesh)
	{
		StaticMesh->SetupAttachment(RootComp);
	}
	ArraySpikes = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Spike");
	if (ArraySpikes)
	{
		ArraySpikes->SetupAttachment(RootComp);
	}
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>("TriggerActivation");
	if (BoxTrigger)
	{
		BoxTrigger->SetupAttachment(RootComp);
	}
}

void ASpikesTrap::BeginPlay()
{
	Super::BeginPlay();
	BoxDmgTrigger = HelperLibrary::GetComponentByName<UBoxComponent>(this, "DamageBox");
	if (BoxDmgTrigger)
	{
		BoxDmgTrigger->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASpikesTrap::OverlapDamage);
	}
	if (BoxTrigger)
	{
		BoxTrigger->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASpikesTrap::OverlapActivation);
		BoxTrigger->OnComponentEndOverlap.AddUniqueDynamic(this, &ASpikesTrap::EndOverlapActivation);
		BoxTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	}
	if (ArraySpikes)
	{
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
		StartingLocation = ArraySpikes->GetComponentLocation();
	}
	if (alwaysActive)
	{
		Activate(nullptr);
	}
}

void ASpikesTrap::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	if (ActivationAuto)
	{
		if (!RetractSpike)
		{
			TimerBeforeTrigger += _deltaTime;

			if (TimerBeforeTrigger >= TimeBetweenActivation)
			{
				TimerSpikes += _deltaTime * Speed;
				if (ArraySpikes)
				{
					ArraySpikes->SetWorldLocation(FMath::Lerp(StartingLocation, EndLocation, TimerSpikes));
				}
				if (TimerSpikes >= 1.0f)
				{
					TimerBeforeTrigger = 0.0f;
					TimerSpikes = 0.0f;
					RetractSpike = true;
				}
			}
		}
		else if (RetractSpike)
		{
			TimerBeforeTrigger += _deltaTime;

			if (TimerBeforeTrigger >= TimeSpikesOut)
			{
				TimerSpikes += _deltaTime * Speed;
				if (ArraySpikes)
				{
					ArraySpikes->SetWorldLocation(FMath::Lerp(EndLocation, StartingLocation, TimerSpikes));
				}
				if (TimerSpikes >= 1.0f)
				{
					TimerBeforeTrigger = 0.0f;
					TimerSpikes = 0.0f;
					RetractSpike = false;
				}
			}
		}
	}
	else
	{

		if (IsTriggered && !RetractSpike)
		{
			TimerBeforeTrigger += _deltaTime;

			if (TimerBeforeTrigger >= TimerTrigger)
			{
				TimerSpikes += _deltaTime * Speed;
				if (ArraySpikes)
				{
					ArraySpikes->SetWorldLocation(FMath::Lerp(StartingLocation, EndLocation, TimerSpikes));
				}
				if (TimerSpikes >= 1.0f)
				{
					IsTriggered = false;
					TimerBeforeTrigger = 0.0f;
					TimerSpikes = 0.0f;
					RetractSpike = true;
				}
			}
		}
		else if (RetractSpike)
		{
			TimerBeforeTrigger += _deltaTime;

			if (TimerBeforeTrigger >= TimerTrigger)
			{
				TimerSpikes += _deltaTime * Speed;
				if (ArraySpikes)
				{
					ArraySpikes->SetWorldLocation(FMath::Lerp(EndLocation, StartingLocation, TimerSpikes));
				}
				if (TimerSpikes >= 1.0f)
				{
					TimerBeforeTrigger = 0.0f;
					TimerSpikes = 0.0f;
					RetractSpike = false;
					if (IsPlayerStillHere)
						IsTriggered = true;
				}
			}
		}
	}
}

void ASpikesTrap::OverlapDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent != nullptr && OtherActor != nullptr && OtherComp != nullptr && (isActivate || ActivationAuto))
	{
		if (AGolemProjectCharacter* character = Cast<AGolemProjectCharacter>(OtherActor))
		{
			character->InflictDamage(damage);
		}
	}
}

void ASpikesTrap::OverlapActivation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent != nullptr && OtherActor != nullptr && OtherComp != nullptr && isActivate)
	{
		if (AGolemProjectCharacter* character = Cast<AGolemProjectCharacter>(OtherActor))
		{
			IsTriggered = true;
			IsPlayerStillHere = true;
		}
	}
}

void ASpikesTrap::EndOverlapActivation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappedComponent != nullptr && OtherActor != nullptr && OtherComp != nullptr && isActivate)
	{
		if (AGolemProjectCharacter* character = Cast<AGolemProjectCharacter>(OtherActor))
		{
			IsPlayerStillHere = false;
		}
	}
}
