// Golem Project - Créajeux 2020


#include "PortalLevel.h"
#include "GolemProjectCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Helpers/HelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APortalLevel::APortalLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(Root);
	BoxDetectionCloseEnough = CreateDefaultSubobject<UBoxComponent>("TriggerDetection");
	BoxDetectionCloseEnough->SetupAttachment(MeshComponent);
	ColliderPortal = CreateDefaultSubobject<UStaticMeshComponent>("ColliderPortal");
	ColliderPortal->SetupAttachment(MeshComponent);
	CenterToReach = CreateDefaultSubobject<USceneComponent>("PointToReach");
	CenterToReach->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void APortalLevel::BeginPlay()
{
	Super::BeginPlay();
	BoxDetectionCloseEnough->OnComponentBeginOverlap.AddDynamic(this, &APortalLevel::OnBeginOverlapDetection);
	BoxDetectionCloseEnough->OnComponentEndOverlap.AddDynamic(this, &APortalLevel::OnEndOverlapDetection);
	CameraComp = HelperLibrary::GetComponentByName<UChildActorComponent>(this, "CameraChild");
}

void APortalLevel::OnBeginOverlapDetection(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent != nullptr && OtherActor != nullptr && OtherComp != nullptr)
	{
		if (AGolemProjectCharacter* character = Cast<AGolemProjectCharacter>(OtherActor))
		{
			IsPlayerCloseEnough = true;
			IsCharacterCloseEnoughEvent(true);
			//character->CanGoToLocation(MeshComponent->GetComponentLocation(), false);
		}
	}
}

void APortalLevel::OnEndOverlapDetection(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappedComp != nullptr && OtherActor != nullptr && OtherComp != nullptr)
	{
		if (AGolemProjectCharacter* character = Cast<AGolemProjectCharacter>(OtherActor))
		{
			IsPlayerCloseEnough = false;
			IsCharacterCloseEnoughEvent(false);
		}
	}
}

void APortalLevel::ActivatePortal()
{
	isPortalActivate = true;
	CharacterTriggeredPortalEvent();
}

// Called every frame
void APortalLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (hasToTargetCharacter)
	{
		FVector targetDirection = targetCharacter->GetActorLocation() - CameraComp->GetComponentLocation();
		CameraComp->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(CameraComp->GetForwardVector(), targetDirection));
		
	}
	/*if (isPortalActivate)
	{
		timerLerp += speedLerp * DeltaTime;
		DisappearRing->SetWorldLocation(FMath::Lerp(lastLocationRing, RingDestination->GetComponentLocation(), timerLerp));
		if (timerLerp >= 1.0f)
		{
			DisappearRing->SetVisibility(false);
			isPortalActivate = false;
			CharacterTriggeredPortalEvent();
		}
	}*/
}

const bool APortalLevel::Interact_Implementation(AActor* caller)
{
	if (AGolemProjectCharacter* character = Cast<AGolemProjectCharacter>(caller))
	{
		if (IsPlayerCloseEnough && IsLevelUnlocked())
		{
			FVector direction = CameraComp->GetComponentLocation() - CenterToReach->GetComponentLocation();
			if (character->CanGoToLocation(CenterToReach->GetComponentLocation(), true, true, direction, true))
			{
				character->OnLocationReach.AddDynamic(this, &APortalLevel::ActivatePortal);
				targetCharacter = character;
				hasToTargetCharacter = true;
				ChangeCamera();
				return true;
			}
		}
	}
	return false;
}

