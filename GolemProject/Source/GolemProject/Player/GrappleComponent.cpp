// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UGrappleComponent::UGrappleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrappleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


void UGrappleComponent::GoToDestination()
{
	FVector velocity = FVector::ZeroVector;
	UWorld* world = GetWorld();

	if (world&&mPawn)
	{
		APlayerController* PlayerController = Cast<APlayerController>(mPawn->GetController());

		FHitResult TraceResult(ForceInit);
		PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, TraceResult);

		//TraceResult.

		/*TArray<FHitResult> hits;
		FVector cameraPos = UGameplayStatics::GetPlayerCameraManager(world, 0)->GetCameraLocation();

		if (world->LineTraceMultiByChannel(hits, cameraPos, , ECollisionChannel::ECC_Visibility)
			mPawn->LaunchPawn(launchForce *velocity, true, true);*/
	}

}

// Called every frame
void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
