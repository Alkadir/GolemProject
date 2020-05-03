// Golem Project - Créajeux 2020


#include "Bumper.h"
#include "GolemProjectCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Helpers/HelperLibrary.h"

// Sets default values
ABumper::ABumper()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(Root);
	MeshBeam = CreateDefaultSubobject<UStaticMeshComponent>("Beam");
	MeshBeam->SetupAttachment(MeshComponent);
	SphereTrigger = CreateDefaultSubobject<USphereComponent>("TriggerActivation");
	SphereTrigger->SetupAttachment(MeshComponent);
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
			FVector velocityToAdd(FVector::ZeroVector);
			FVector directionLaunch = MeshComponent->GetUpVector();
			if (KeepLastVelocity)
			{
				velocityToAdd.X = directionLaunch.X >= 0.0f ? character->GetVelocity().X : -character->GetVelocity().X;
				velocityToAdd.Y = directionLaunch.Y >= 0.0f ? character->GetVelocity().Y : -character->GetVelocity().Y;
				velocityToAdd.Z = directionLaunch.Z >= 0.0f ? -character->GetVelocity().Z : character->GetVelocity().Z;
			}
			//FVector 
			character->LaunchCharacter(directionLaunch * ForceLaunch + velocityToAdd, false, true);
			if (character->GetDashComponent())
			{
				character->GetDashComponent()->ResetDashInAir();
			}
		}
	}
}

// Called every frame
void ABumper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

