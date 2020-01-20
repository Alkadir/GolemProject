// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GolemProjectCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/DashComponent.h"
#include <Engine/Engine.h>
#include "Player/GrappleComponent.h"
#include "Blueprint/UserWidget.h"
#include "Helpers/HelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/Targetable.h"
#include "Camera/PlayerCameraManager.h"


//////////////////////////////////////////////////////////////////////////
// AGolemProjectCharacter

AGolemProjectCharacter::AGolemProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1620.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm




	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGolemProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Input left Mouse Click
	PlayerInputComponent->BindAction("Fire1", IE_Released, this, &AGolemProjectCharacter::Fire);

	PlayerInputComponent->BindAction("Fire2", IE_Pressed, this, &AGolemProjectCharacter::ChangeCamera);
	PlayerInputComponent->BindAction("Fire2", IE_Released, this, &AGolemProjectCharacter::ChangeCamera);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGolemProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGolemProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGolemProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGolemProjectCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGolemProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGolemProjectCharacter::TouchStopped);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AGolemProjectCharacter::Dash);
}

void AGolemProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	currentSightWidget = CreateWidget(GetWorld(), sightHudClass);
	dashComponent = FindComponentByClass<UDashComponent>();
	mGrapple = FindComponentByClass<UGrappleComponent>();
	sightCamera = HelperLibrary::GetComponentByName<UChildActorComponent>(this, "ShoulderCamera");
	initialGroundFriction = GetCharacterMovement()->GroundFriction;

	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc)
	{
		PlayerCameraManager = pc->PlayerCameraManager;
		pc->bShowMouseCursor = showCursor;
	}
}

void AGolemProjectCharacter::Dash()
{
	if (dashComponent != nullptr)
	{
		if (Controller != NULL)
		{
			FVector direction = GetLastMovementInputVector();

			if (m_valueForward == 0.0f && m_valueRight == 0.0f)
			{
				direction = GetActorForwardVector();
			}
			direction.Normalize();
			dashComponent->Dash(direction);
		}
	}
}

void AGolemProjectCharacter::CheckElementTargetable()
{
	TArray<AActor*> Actors;
	TArray<AActor*> actorCloseEnough;
	if (UWorld * world = GetWorld())
	{
		UGameplayStatics::GetAllActorsWithInterface(world, UTargetable::StaticClass(), Actors);
		for (AActor* actor : Actors)
		{
			if (!actor->Implements<UTargetable>()) continue;

			if (FVector::DistSquared(actor->GetActorLocation(), GetActorLocation()) < 1000.0f * 1000.0f)
			{
				actorCloseEnough.Add(actor);
			}
		}
		HelperLibrary::SortActorsByDistanceTo(actorCloseEnough, this);
		for (AActor* actor : actorCloseEnough)
		{
			// > 0 object seen
			FVector FromSoftware = (actor->GetActorLocation() - PlayerCameraManager->GetCameraLocation()).GetSafeNormal();
			if (FVector::DotProduct(FollowCamera->GetForwardVector(), FromSoftware) > 0.0f)
			{
				FHitResult hitResult;

				if (world->LineTraceSingleByChannel(hitResult, GetActorLocation(), actor->GetActorLocation(), ECollisionChannel::ECC_Visibility))
				{
					//HelperLibrary::Print(2.0f, hitResult.GetActor()->GetName());
					ITargetable* target = Cast<ITargetable>(hitResult.GetActor());
					if (target != nullptr)
					{
						ClosestGrapplingHook = actor;
						break;
					}
				}
			}
		}
	}
}

void AGolemProjectCharacter::Fire()
{
	CheckElementTargetable();
	if (mGrapple)
	{
		mGrapple->Cancel();

		if(isSightCameraEnabled)
			mGrapple->GoToDestination();
	}
}

void AGolemProjectCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AGolemProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AGolemProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AGolemProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGolemProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGolemProjectCharacter::ChangeCamera()
{
	if (sightCamera)
	{
		APlayerController* pc = Cast<APlayerController>(GetController());
		if (pc)
		{
			if (!isSightCameraEnabled)
			{
				isSightCameraEnabled = true;
				GetCharacterMovement()->bOrientRotationToMovement = false;
				pc->SetViewTargetWithBlend(sightCamera->GetChildActor(), 0.25f);

				if (currentSightWidget)
					currentSightWidget->AddToViewport();

			}
			else
			{
				if (currentSightWidget)
					currentSightWidget->RemoveFromViewport();

				isSightCameraEnabled = false;
				GetCharacterMovement()->bOrientRotationToMovement = true;
				pc->SetViewTargetWithBlend(this, 0.25f);
			}

		}
	}
}

void AGolemProjectCharacter::MoveForward(float Value)
{
	m_valueForward = Value;
	if ((Controller != NULL) && (Value != 0.0f))
	{

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		if (isSightCameraEnabled || mGrapple->GetProjectile())
		{
			Direction = mGrapple->GetDirection();
		}

		AddMovementInput(Direction, Value);
	}
}

void AGolemProjectCharacter::MoveRight(float Value)
{
	m_valueRight = Value;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction

		AddMovementInput(Direction, Value);
	}
}

void AGolemProjectCharacter::ResetFriction()
{
	GetCharacterMovement()->GroundFriction = initialGroundFriction;
}
