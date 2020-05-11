// Golem Project - Créajeux 2020


#include "PortalLevel.h"
#include "GolemProjectCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Helpers/HelperLibrary.h"

// Sets default values
APortalLevel::APortalLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(Root);
	TriggerChangeLevel = CreateDefaultSubobject<UStaticMeshComponent>("TriggerActivation");
	TriggerChangeLevel->SetupAttachment(MeshComponent);
	BoxDetectionCloseEnough = CreateDefaultSubobject<UBoxComponent>("TriggerDetection");
	BoxDetectionCloseEnough->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void APortalLevel::BeginPlay()
{
	Super::BeginPlay();
	TriggerChangeLevel->OnComponentBeginOverlap.AddDynamic(this, &APortalLevel::OnBeginOverlap);
	BoxDetectionCloseEnough->OnComponentBeginOverlap.AddDynamic(this, &APortalLevel::OnBeginOverlapDetection);
	BoxDetectionCloseEnough->OnComponentEndOverlap.AddDynamic(this, &APortalLevel::OnEndOverlapDetection);
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

void APortalLevel::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent != nullptr && OtherActor != nullptr && OtherComp != nullptr)
	{
		if (AGolemProjectCharacter* character = Cast<AGolemProjectCharacter>(OtherActor))
		{
			CharacterTriggeredPortalEvent();
		}
	}
}

// Called every frame
void APortalLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

const bool APortalLevel::Interact_Implementation(AActor* caller)
{
	if (AGolemProjectCharacter* character = Cast<AGolemProjectCharacter>(caller))
	{
		if (IsPlayerCloseEnough)
		{
			character->CanGoToLocation(MeshComponent->GetComponentLocation(), true);
			return true;
		}
	}
	return false;
}

