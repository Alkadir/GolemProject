// Golem Project - Créajeux 2020
#include "SkillItemActivator.h"
#include "Components/StaticMeshComponent.h"
#include "GolemProjectCharacter.h"
// Sets default values
ASkillItemActivator::ASkillItemActivator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	staticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("mesh");

	if (staticMeshComp)
		staticMeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASkillItemActivator::BeginPlay()
{
	Super::BeginPlay();
	staticMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ASkillItemActivator::BeginOverlap);
}

// Called every frame
void ASkillItemActivator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkillItemActivator::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGolemProjectCharacter* character = Cast<AGolemProjectCharacter>(OtherActor))
	{
		if (fist)
		{
			character->SetFistSkillEnabled(true);
			character->ChangeToFist();
		}
		if (grapple)
		{
			character->SetGrappleSkillEnabled(true);
			character->ChangeToGrapple();
		}

		Destroy();
	}
}