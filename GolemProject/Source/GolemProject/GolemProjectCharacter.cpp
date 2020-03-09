// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GolemProjectCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include <Engine/Engine.h>
#include "Player/GrappleComponent.h"
#include "Blueprint/UserWidget.h"
#include "Helpers/HelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/Targetable.h"
#include "Camera/PlayerCameraManager.h"
#include "GolemProjectGameMode.h"
#include "Player/HealthComponent.h"
#include "Player/PushingComponent.h"
#include "Interfaces/Interactable.h"
#include "Player/FistComponent.h"
#include "Player/SwingPhysics.h"

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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGolemProjectCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Input left Mouse Click
	PlayerInputComponent->BindAction("Fire1", IE_Released, this, &AGolemProjectCharacter::Fire);

	PlayerInputComponent->BindAction("Fire2", IE_Pressed, this, &AGolemProjectCharacter::ChangeCamera);
	PlayerInputComponent->BindAction("Fire2", IE_Released, this, &AGolemProjectCharacter::ChangeCamera);
	PlayerInputComponent->BindAction("ChangeToGrapple", IE_Pressed, this, &AGolemProjectCharacter::ChangeToGrapple);
	PlayerInputComponent->BindAction("ChangeToFist", IE_Pressed, this, &AGolemProjectCharacter::ChangeToFist);

	PlayerInputComponent->BindAction("AssistedGrapple", IE_Pressed, this, &AGolemProjectCharacter::UseAssistedGrapple);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGolemProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGolemProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGolemProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGolemProjectCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AGolemProjectCharacter::Dash);

	//PlayerInputComponent->BindAction("Interacte", IE_Pressed, this, &AGolemProjectCharacter::Interact);
}

void AGolemProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	//currentSightWidget = CreateWidget(GetWorld(), sightHudClass);
	dashComponent = FindComponentByClass<UDashComponent>();
	mGrapple = FindComponentByClass<UGrappleComponent>();
	FistComp = FindComponentByClass<UFistComponent>();
	HealthComponent = FindComponentByClass<UHealthComponent>();
	PushingComponent = FindComponentByClass<UPushingComponent>();
	sightCamera = HelperLibrary::GetComponentByName<UChildActorComponent>(this, "ShoulderCamera");
	sightCameraL = HelperLibrary::GetComponentByName<UChildActorComponent>(this, "ShoulderCameraL");
	initialGroundFriction = GetCharacterMovement()->GroundFriction;
	APlayerController* pc = Cast<APlayerController>(GetController());
	HealthComponent->SetLastPositionGrounded(GetActorLocation());
	if (pc)
	{
		pc->bShowMouseCursor = showCursor;
	}

	mGrapple->IsTargetingGrapple = true;
	FistComp->IsTargetingFist = false;
}

void AGolemProjectCharacter::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	if (UWorld * world = GetWorld())
	{
		FHitResult hit;
		float height = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 1.0f;

		if (world->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() - FVector::UpVector * height, ECollisionChannel::ECC_Visibility))
		{
			if (hit.bBlockingHit)
			{
				if (mGrapple)
				{
					if (mGrapple->GetSwingPhysics())
					{
						mGrapple->StopSwingPhysics();
					}
				}
			}
		}
	}

	if (mGrapple && !mGrapple->GetSwingPhysics())
	{
		FRotator rotFinal = FRotator::ZeroRotator;
		rotFinal.Pitch = 0.0f;
		rotFinal.Yaw = GetActorRotation().Yaw;
		rotFinal.Roll = GetActorRotation().Roll;

		FRotator rot = FMath::Lerp(GetActorRotation(), rotFinal, 0.05f);
		SetActorRotation(rot);
	}
}

void AGolemProjectCharacter::Jump()
{
	if (!PushingComponent->GetIsPushingObject())
	{
		Super::Jump();
	}

	if (mGrapple)
	{
		if (mGrapple->GetSwingPhysics())
		{
			mGrapple->StopSwingPhysics();
		}
	}
}

void AGolemProjectCharacter::Dash()
{
	if (mGrapple != nullptr && !mGrapple->GetIsFiring() && dashComponent != nullptr && !PushingComponent->GetIsPushingObject())
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

void AGolemProjectCharacter::UseAssistedGrapple()
{
	if (mGrapple != nullptr)
	{
		mGrapple->GoToDestination(true);
	}
}

void AGolemProjectCharacter::ChangeToGrapple()
{
	if (mGrapple->IsTargetingGrapple) return;
	mGrapple->IsTargetingGrapple = true;
	FistComp->IsTargetingFist = false;
	APlayerController* pc = Cast<APlayerController>(GetController());
	if (isSightCameraEnabled && pc)
	{
		pc->SetViewTargetWithBlend(sightCamera->GetChildActor(), 0.25f);
	}
}

void AGolemProjectCharacter::ChangeToFist()
{
	if (FistComp->IsTargetingFist) return;
	FistComp->IsTargetingFist = true;
	mGrapple->IsTargetingGrapple = false;
	APlayerController* pc = Cast<APlayerController>(GetController());
	if (isSightCameraEnabled && pc)
	{
		pc->SetViewTargetWithBlend(sightCameraL->GetChildActor(), 0.25f);
	}
}

void AGolemProjectCharacter::Fire()
{
	if (PushingComponent->GetIsPushingObject())
	{
		return;
	}
	if (mGrapple && mGrapple->IsTargetingGrapple)
	{
		mGrapple->Cancel();

		if (isSightCameraEnabled)
			mGrapple->GoToDestination(false);
	}
	else if (FistComp && FistComp->IsTargetingFist)
	{
		if (isSightCameraEnabled)
			FistComp->GoToDestination();
	}
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
	if (PushingComponent->GetIsPushingObject())
	{
		return;
	}
	if (sightCamera)
	{
		APlayerController* pc = Cast<APlayerController>(GetController());
		if (pc)
		{
			if (!isSightCameraEnabled)
			{
				isSightCameraEnabled = true;
				GetCharacterMovement()->bOrientRotationToMovement = false;
				if (mGrapple->IsTargetingGrapple)
					pc->SetViewTargetWithBlend(sightCamera->GetChildActor(), 0.25f);
				else if (FistComp->IsTargetingFist)
					pc->SetViewTargetWithBlend(sightCameraL->GetChildActor(), 0.25f);

				/*	if (currentSightWidget && !currentSightWidget->IsInViewport() && !mGrapple->GetProjectile())
						currentSightWidget->AddToViewport();*/

			}
			else
			{
				/*if (currentSightWidget && currentSightWidget->IsInViewport())
					currentSightWidget->RemoveFromViewport();*/

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

		if (!PushingComponent->GetIsPushingObject())
		{
			if (mGrapple->GetSwingPhysics())
			{
				mGrapple->GetSwingPhysics()->AddForceMovement(FollowCamera->GetForwardVector() * m_valueForward);
			}
			else
			{
			if (mGrapple->IsTargetingGrapple && (isSightCameraEnabled || mGrapple->GetProjectile()))
			{
				Direction = mGrapple->GetDirection();
			}
			else if (FistComp->IsTargetingFist && isSightCameraEnabled)
			{
				Direction = FistComp->GetDirection();
			}
		}
		}
		else
		{
			Direction = PushingComponent->GetPushingDirection();
		}
		AddMovementInput(Direction, Value);
	}
}

void AGolemProjectCharacter::MoveRight(float Value)
{
	if (PushingComponent->GetIsPushingObject())
	{
		return;
	}
	m_valueRight = Value;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if (mGrapple->GetSwingPhysics())
		{
			mGrapple->GetSwingPhysics()->AddForceMovement(FollowCamera->GetRightVector() * m_valueRight);
		}
		else
		{
			// add movement in that direction
			if (isSightCameraEnabled &&
				(mGrapple->IsTargetingGrapple || mGrapple->GetProjectile()) && !GetCharacterMovement()->IsFalling())
			{
				Direction = mGrapple->GetDirection();

				float X = Direction.X;
				float Y = Direction.Y;
				float Z = Direction.Z;

			Direction.X = -Y;
			Direction.Y = X;
		}
		else if (isSightCameraEnabled && FistComp->IsTargetingFist)
		{
			Direction = FistComp->GetDirection();

				float X = Direction.X;
				float Y = Direction.Y;
				float Z = Direction.Z;

				Direction.X = -Y;
				Direction.Y = X;
			}
			AddMovementInput(Direction, Value);
		}
	}
}

void AGolemProjectCharacter::ResetFriction()
{
	GetCharacterMovement()->GroundFriction = initialGroundFriction;
}

void AGolemProjectCharacter::PushBloc(FVector pushingDirection, FVector pushingPosition, FRotator pushingRotation)
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	PushingComponent->PushBloc(pushingDirection, pushingPosition, pushingRotation);
}


void AGolemProjectCharacter::StopPushBloc()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	PushingComponent->StopPushBloc();
}
