// Golem Project - Créajeux 2020


#include "CheckPoint.h"
#include "Components/BoxComponent.h"
#include "GolemProjectCharacter.h"
#include "Player/HealthComponent.h"
#include "Components/SceneComponent.h"
#include "Helpers/HelperLibrary.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
	TriggerSave = CreateDefaultSubobject<UBoxComponent>("TriggerSave");
	RootComponent = TriggerSave;
	HasAlreadyBeenUsed = false;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	TriggerSave->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::BeginOverlap);
}

void ACheckPoint::BeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (HitComp != nullptr && OtherActor != nullptr && OtherComp != nullptr && !HasAlreadyBeenUsed)
	{
		if (AGolemProjectCharacter* character = Cast<AGolemProjectCharacter>(OtherActor))
		{
			if (UHealthComponent* healthComponent = character->GetHealthComponent())
			{
				healthComponent->SetPositionCheckPoint(GetActorLocation());
				HasAlreadyBeenUsed = true;
			}
		}
	}
}

