// Golem Project - Créajeux 2020


#include "RaycastingComponent.h"
#include "GolemProjectCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Helpers/HelperLibrary.h"
#include "Player/HealthComponent.h"
#include "DrawDebugHelpers.h"
#include "Objects/MovingPlatform.h"
#include "Math/UnrealMathUtility.h"

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
	LeftFootOffset = 0.0f;
	RightFootOffset = 0.0f;
}

float URaycastingComponent::LineForIK(FName _boneName)
{
	if (UWorld* world = GetWorld())
	{
		if (MeshComponent != nullptr && Character != nullptr && Character->GetCapsuleComponent() != nullptr)
		{
			BoneLocation = MeshComponent->GetSocketLocation(_boneName);

			float capsuleSize = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			FHitResult hitResult;
			FVector locationStart(BoneLocation.X, BoneLocation.Y, Character->GetActorLocation().Z);
			FVector locationEnd(BoneLocation.X, BoneLocation.Y, Character->GetActorLocation().Z - (capsuleSize + MaxDistanceBelowCapsule));
			if (world->LineTraceSingleByChannel(hitResult, locationStart, locationEnd, ECollisionChannel::ECC_Visibility))
			{
				FVector diffBetweenStartAndCollision = locationStart - hitResult.Location;
				float Offset = diffBetweenStartAndCollision.Size() - capsuleSize;
				return Offset;
			}
		}
	}
	return 0;
}


// Called every frame
void URaycastingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	LeftFootOffset = /*LineForIK(LeftFootName) * -1.0f;*/ FMath::FInterpTo(LeftFootOffset, LineForIK(LeftFootName), DeltaTime, 15.0f);

	RightFootOffset =/* LineForIK(RightFootName) * -1.0f;*/ FMath::FInterpTo(RightFootOffset, LineForIK(RightFootName), DeltaTime, 15.0f);
	PelvisOffset = LeftFootOffset > RightFootOffset ? LeftFootOffset : RightFootOffset;

	//will be implemented later
	/*if (UWorld* world = GetWorld())
	{
		if (MeshComponent != nullptr)
		{
			FootRGrounded = false;
			FootLGrounded = false;
			FHitResult hitResult;
			FVector locationStart = MeshComponent->GetBoneTransform(indexFootR).GetLocation();
			if (world->LineTraceSingleByChannel(hitResult, locationStart, locationStart + FVector::UpVector * -1 * 30.0f, ECollisionChannel::ECC_Visibility))
			{
				if (hitResult.GetActor() == nullptr)
					HelperLibrary::Print("no actor");
				//AMovingPlatform* movingPlatform = Cast<AMovingPlatform>(hitResult.GetActor()->ActorHasTag("Test"));
				if (hitResult.GetActor() !=nullptr && hitResult.GetActor()->ActorHasTag("Test"))
					HelperLibrary::Print("holaaa");
				//FootRGrounded = true;
			}
			locationStart = MeshComponent->GetBoneTransform(indexFootL).GetLocation();
			if (world->LineTraceSingleByChannel(hitResult, locationStart, locationStart + FVector::UpVector * -1 * 30.0f, ECollisionChannel::ECC_GameTraceChannel2))
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
	}*/
}

