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
#include "Kismet/KismetMathLibrary.h"

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
	IsGroundFlat = true;
}

float URaycastingComponent::CalculateOffsetIK(FName _boneName, FName _boneName2, FVector& _floorNormal)
{
	if (UWorld* world = GetWorld())
	{
		if (MeshComponent != nullptr && Character != nullptr && Character->GetCapsuleComponent() != nullptr)
		{
			BoneLocation = MeshComponent->GetSocketLocation(_boneName);

			float capsuleSize = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			FHitResult hitResult;
			//start from bone position but in the middle of the character
			FVector locationStart(BoneLocation.X, BoneLocation.Y, Character->GetActorLocation().Z);
			//end from lower than the capsule
			FVector locationEnd(BoneLocation.X, BoneLocation.Y, Character->GetActorLocation().Z - (capsuleSize + MaxDistanceBelowCapsule));
			FVector tmpFloorNormal;
			float Offset = 0.0f;

			if (world->LineTraceSingleByChannel(hitResult, Character->GetActorLocation(), Character->GetActorLocation() - (capsuleSize + MaxDistanceBelowCapsule), ECollisionChannel::ECC_Visibility))
			{
				IsGroundFlat = hitResult.Normal == FVector::UpVector;
			}
			if (world->LineTraceSingleByChannel(hitResult, locationStart, locationEnd, ECollisionChannel::ECC_Visibility))
			{
				FVector diffBetweenStartAndCollision = locationStart - hitResult.Location;
				Offset = diffBetweenStartAndCollision.Size() - capsuleSize;
				tmpFloorNormal = hitResult.ImpactNormal;
				//return Offset;
			}
			BoneLocation = MeshComponent->GetSocketLocation(_boneName2);
			locationStart = FVector(BoneLocation.X, BoneLocation.Y, Character->GetActorLocation().Z);
			//end from lower than the capsule
			locationEnd = FVector(BoneLocation.X, BoneLocation.Y, Character->GetActorLocation().Z - (capsuleSize + MaxDistanceBelowCapsule));
			if (world->LineTraceSingleByChannel(hitResult, locationStart, locationEnd, ECollisionChannel::ECC_Visibility))
			{
				FVector diffBetweenStartAndCollision2 = locationStart - hitResult.Location;
				float Offset2 = diffBetweenStartAndCollision2.Size() - capsuleSize;
				if (Offset2 < Offset)
				{
					_floorNormal = hitResult.ImpactNormal;
					return Offset2;
				}
				else
				{
					_floorNormal = tmpFloorNormal;
					return Offset;
				}
			}
		}
	}
	return 0;
}

FRotator URaycastingComponent::CalculateRotationOffsetFoot(FVector _floorNormal)
{
	if (Character != nullptr)
	{
		FVector upVectorCharacter = Character->GetActorUpVector();
		//calculate the angle between up to the foot and the normal
		float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(upVectorCharacter, _floorNormal)));
		//calculate the axis to rotate the foot
		FVector axis = FVector::CrossProduct(upVectorCharacter, _floorNormal);
		return UKismetMathLibrary::RotatorFromAxisAndAngle(axis, angle);
	}
	return FRotator(0.0f, 0.0f, 0.0f);
}


// Called every frame
void URaycastingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	LeftFootOffset = FMath::FInterpTo(LeftFootOffset, CalculateOffsetIK(LeftFootName, LeftFoot2Name, FloorNormalLeft), DeltaTime, InterpSpeed)/* + 1.5f*/;
	RightFootOffset = FMath::FInterpTo(RightFootOffset, CalculateOffsetIK(RightFootName, RightFoot2Name, FloorNormalRight), DeltaTime, InterpSpeed)/*-  1.5f*/;
	PelvisOffset = (LeftFootOffset > RightFootOffset ? FMath::FInterpTo(PelvisOffset, LeftFootOffset + 1.0f, DeltaTime, InterpSpeed) : FMath::FInterpTo(PelvisOffset, RightFootOffset + 1.0f, DeltaTime, InterpSpeed));
	//PelvisOffset = 

	LeftFootRotationOffset = FMath::RInterpTo(LeftFootRotationOffset, CalculateRotationOffsetFoot(FloorNormalLeft), DeltaTime, InterpSpeed);
	RightFootRotationOffset = FMath::RInterpTo(RightFootRotationOffset, CalculateRotationOffsetFoot(FloorNormalRight), DeltaTime, InterpSpeed);
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

