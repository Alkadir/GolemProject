// Golem Project - Créajeux 2020


#include "RaycastingComponent.h"
#include "GolemProjectCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Helpers/HelperLibrary.h"
#include "Player/HealthComponent.h"
#include "DrawDebugHelpers.h"
#include "Objects/MovingPlatform.h"

// Sets default values for this component's properties
URaycastingComponent::URaycastingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URaycastingComponent::BeginPlay()
{
	Super::BeginPlay();
	if (AActor* owner = GetOwner())
	{
		Character = Cast<AGolemProjectCharacter>(owner);
		if (Character != nullptr)
		{
			MeshComponent = Character->GetMesh();
			if (MeshComponent != nullptr)
			{
				indexFootL = MeshComponent->GetBoneIndex("ball_l");
				indexFootR = MeshComponent->GetBoneIndex("ball_r");
			}
		}
	}

}


// Called every frame
void URaycastingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (UWorld* world = GetWorld())
	{
		if (MeshComponent != nullptr)
		{
			FootRGrounded = false;
			FootLGrounded = false;
			FHitResult hitResult;
			FVector locationStart = MeshComponent->GetBoneTransform(indexFootR).GetLocation();
			if (world->LineTraceSingleByChannel(hitResult, locationStart, locationStart + FVector::UpVector * -1 * 30.0f, ECollisionChannel::ECC_Visibility))
			{
				AMovingPlatform* movingPlatform = Cast<AMovingPlatform>(hitResult.GetActor());
				if (movingPlatform == nullptr)
					FootRGrounded = true;
			}
			locationStart = MeshComponent->GetBoneTransform(indexFootL).GetLocation();
			if (world->LineTraceSingleByChannel(hitResult, locationStart, locationStart + FVector::UpVector * -1 * 30.0f, ECollisionChannel::ECC_Visibility))
			{
				AMovingPlatform* movingPlatform = Cast<AMovingPlatform>(hitResult.GetActor());
				if (movingPlatform == nullptr)
					FootLGrounded = true;
			}

			if (FootLGrounded && FootRGrounded && Character->GetHealthComponent() != nullptr)
			{
				Character->GetHealthComponent()->SetLastPositionGrounded(Character->GetActorLocation());
			}
		}
	}
}

