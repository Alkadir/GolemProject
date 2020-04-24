// Golem Project - Créajeux 2020


#include "Bumper.h"
#include "GolemProjectCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
ABumper::ABumper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(Root);
	SphereTrigger = CreateDefaultSubobject<USphereComponent>("TriggerActivation");
	SphereTrigger->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ABumper::BeginPlay()
{
	Super::BeginPlay();
	SphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABumper::OnBeginOverlap);
}

void ABumper::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent != nullptr && OtherActor != nullptr && OtherComp != nullptr)
	{
		if (AGolemProjectCharacter* character = Cast<AGolemProjectCharacter>(OtherActor))
		{
			FVector directionLaunch = MeshComponent->GetUpVector();
			character->LaunchCharacter(directionLaunch * ForceLaunch, false, true);
		}
	}
}

// Called every frame
void ABumper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

