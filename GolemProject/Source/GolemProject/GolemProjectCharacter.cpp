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
#include "GolemProjectGameMode.h"
#include "Player/HealthComponent.h"
#include "Interfaces/Interactable.h"

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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGolemProjectCharacter::Interact);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Input left Mouse Click
	PlayerInputComponent->BindAction("Fire1", IE_Released, this, &AGolemProjectCharacter::Fire);

	PlayerInputComponent->BindAction("Fire2", IE_Pressed, this, &AGolemProjectCharacter::ChangeCamera);
	PlayerInputComponent->BindAction("Fire2", IE_Released, this, &AGolemProjectCharacter::ChangeCamera);

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

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AGolemProjectCharacter::Interact);
}

void AGolemProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	currentSightWidget = CreateWidget(GetWorld(), sightHudClass);
	dashComponent = FindComponentByClass<UDashComponent>();
	mGrapple = FindComponentByClass<UGrappleComponent>();
	HealthComponent = FindComponentByClass<UHealthComponent>();
	sightCamera = HelperLibrary::GetComponentByName<UChildActorComponent>(this, "ShoulderCamera");
	initialGroundFriction = GetCharacterMovement()->GroundFriction;
	APlayerController* pc = Cast<APlayerController>(GetController());
	HealthComponent->SetLastPositionGrounded(GetActorLocation());
	if (pc)
	{
		pc->bShowMouseCursor = showCursor;
	}
}

void AGolemProjectCharacter::Jump()
{
	if (!isPushing)
	{
		Super::Jump();
	}
}

void AGolemProjectCharacter::Dash()
{
	if (mGrapple != nullptr && !mGrapple->GetIsFiring() && dashComponent != nullptr && !isPushing)
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

void AGolemProjectCharacter::Interact()
{
	if (toInteract != nullptr)
	{
		toInteract->Interact_Implementation(this);
	}
}

void AGolemProjectCharacter::PushBloc()
{
	isPushing = !isPushing;

	float blocAngle = 0.0f;
	AActor* actorToInteract = Cast<AActor>(toInteract);

	if (isPushing && actorToInteract != nullptr)
	{
		FVector blocDelta = actorToInteract->GetActorLocation() - GetActorLocation();

		float angleDelta = FMath::Atan2(blocDelta.X, blocDelta.Y);
		angleDelta = FMath::RadiansToDegrees(angleDelta) - 90.0f;

		if (angleDelta >= -45.0f && angleDelta <= 45.0f)
		{
			blocAngle = 0.0f;
		}
		else if (angleDelta >= -135.0f && angleDelta <= -45.0f)
		{
			blocAngle = 90.0f;
		}
		else if (angleDelta >= -225.0f && angleDelta <= -135.0f)
		{
			blocAngle = 180.0f;
		}
		else if ((angleDelta >= -315.0f && angleDelta <= -225.0f) || (angleDelta >= 45.0f && angleDelta <= 90.0f))
		{
			blocAngle = 270.0f;
		}

		UE_LOG(LogTemp, Warning, TEXT("%f"), angleDelta);

		SetActorRotation(FRotator(0.0f, blocAngle, 0.0f));
	}
}

void AGolemProjectCharacter::Fire()
{
	if (mGrapple)
	{
		mGrapple->Cancel();

		if (isSightCameraEnabled)
			mGrapple->GoToDestination(false);
	}
}

void AGolemProjectCharacter::TurnAtRate(float Rate)
{
	if (!isPushing)
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AGolemProjectCharacter::LookUpAtRate(float Rate)
{
	if (!isPushing)
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AGolemProjectCharacter::ChangeCamera()
{
	if (sightCamera && !isPushing)
	{
		APlayerController* pc = Cast<APlayerController>(GetController());
		if (pc)
		{
			if (!isSightCameraEnabled)
			{
				isSightCameraEnabled = true;
				GetCharacterMovement()->bOrientRotationToMovement = false;
				pc->SetViewTargetWithBlend(sightCamera->GetChildActor(), 0.25f);

				if (currentSightWidget && !currentSightWidget->IsInViewport() && !mGrapple->GetProjectile())
					currentSightWidget->AddToViewport();

			}
			else
			{
				if (currentSightWidget && currentSightWidget->IsInViewport())
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

		if (isPushing)
		{
			GetCharacterMovement()->bOrientRotationToMovement = false;
			AddMovementInput(GetActorForwardVector(), Value);
			//UE_LOG(LogTemp, Log, TEXT("Forward : %s - Value : %f"), *GetActorForwardVector().ToString(), Value);
		}
		else
		{
			GetCharacterMovement()->bOrientRotationToMovement = true;
			AddMovementInput(Direction, Value);
		}
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
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		if (isSightCameraEnabled || mGrapple->GetProjectile() && !isPushing)
		{
			Direction = mGrapple->GetDirection();

			float X = Direction.X;
			float Y = Direction.Y;
			float Z = Direction.Z;

			Direction.X = -Y;
			Direction.Y = X;
		}
		AddMovementInput(Direction, Value);
	}
}

void AGolemProjectCharacter::ResetFriction()
{
	GetCharacterMovement()->GroundFriction = initialGroundFriction;
}
