// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GolemProjectCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
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
#include "Player/SwingPhysic.h"
#include "Player/RaycastingComponent.h"
#include "Objects/PushableBloc.h"

//////////////////////////////////////////////////////////////////////////
// AGolemProjectCharacter

AGolemProjectCharacter::AGolemProjectCharacter()
{
	// Set size for collision capsule
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	}

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 1620.0f, 0.0f); // ...at this rotation rate
		GetCharacterMovement()->JumpZVelocity = 600.f;
		GetCharacterMovement()->AirControl = 0.2f;
	}

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	if (CameraBoom)
	{
		CameraBoom->SetupAttachment(RootComponent);
		CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
		CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	}

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	if (FollowCamera)
	{
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
		FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	}

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

	PlayerInputComponent->BindAction("Fire2", IE_Pressed, this, &AGolemProjectCharacter::ChangeCameraPressed);
	PlayerInputComponent->BindAction("Fire2", IE_Released, this, &AGolemProjectCharacter::ChangeCameraReleased);
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
	RaycastingComponent = FindComponentByClass<URaycastingComponent>();
	if (GetCharacterMovement())
	{
		initialGroundFriction = GetCharacterMovement()->GroundFriction;
	}
	pc = Cast<APlayerController>(GetController());
	if (HealthComponent)
	{
		HealthComponent->SetLastPositionGrounded(GetActorLocation());
	}
	if (pc)
	{
		pc->bShowMouseCursor = showCursor;
	}
	if (PushingComponent)
	{
		PushingComponent->OnStartPushingObject.AddDynamic(this, &AGolemProjectCharacter::SetUpBlockOffsetPositon);
	}

	if (mGrapple)
		mGrapple->IsTargetingGrapple = true;

	if (FistComp)
		FistComp->IsTargetingFist = false;

	IsInteractingOrAiming = false;


}

void AGolemProjectCharacter::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	if (mGrapple && !mGrapple->GetSwingPhysics())
	{
		FRotator rotFinal = FRotator::ZeroRotator;
		rotFinal.Pitch = 0.0f;
		rotFinal.Yaw = GetActorRotation().Yaw;
		rotFinal.Roll = GetActorRotation().Roll;

		FRotator rot = FMath::Lerp(GetActorRotation(), rotFinal, 0.05f);
		SetActorRotation(rot);
	}
	if (PushingComponent && PushingComponent->GetIsPushingObject(false) && actorToInteract)
	{
		if (fabs(startPushingZ - GetActorLocation().Z) > 2.f)
		{
			HelperLibrary::Print(FString::SanitizeFloat(fabs(startPushingZ - GetActorLocation().Z)));
			StopPushBloc();
		}
		else
		{
			actorToInteract->SetActorLocation(GetActorLocation() + PushingComponent->GetBlockOffsetPosition());
			rightHandPosition = GetActorLocation() + offsetRightHand;
			leftHandPosition = GetActorLocation() + offsetLeftHand;
		}
	}
}

void AGolemProjectCharacter::Jump()
{
	if (PushingComponent && !PushingComponent->GetIsPushingObject())
	{
		Super::Jump();
	}

	if (mGrapple && mGrapple->GetSwingPhysics())
	{
		mGrapple->StopSwingPhysics();
	}
}

void AGolemProjectCharacter::Dash()
{
	if (mGrapple && !mGrapple->GetIsFiring() && dashComponent && !PushingComponent->GetIsPushingObject())
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
	if (isGrappleSkillEnabled && mGrapple)
	{
		ChangeToGrapple();
		mGrapple->GoToDestination(true);
	}
}

void AGolemProjectCharacter::ChangeToGrapple()
{
	if (!isGrappleSkillEnabled || FistComp == nullptr || mGrapple == nullptr || mGrapple->IsTargetingGrapple) return;
	mGrapple->IsTargetingGrapple = true;
	FistComp->IsTargetingFist = false;
	if (isSightCameraEnabled && pc && sightCamera)
	{
		pc->SetViewTargetWithBlend(sightCamera->GetChildActor(), 0.25f);
	}
}

void AGolemProjectCharacter::ChangeToFist()
{
	if (!isFistSkillEnabled || FistComp == nullptr || mGrapple == nullptr || FistComp && FistComp->IsTargetingFist) return;
	FistComp->IsTargetingFist = true;
	mGrapple->IsTargetingGrapple = false;
	if (isSightCameraEnabled && pc && sightCameraL)
	{
		pc->SetViewTargetWithBlend(sightCameraL->GetChildActor(), 0.25f);
	}
}

void AGolemProjectCharacter::Fire()
{
	if (PushingComponent && PushingComponent->GetIsPushingObject())
	{
		return;
	}
	if (isGrappleSkillEnabled && mGrapple && mGrapple->IsTargetingGrapple)
	{
		mGrapple->Cancel();

		if (isSightCameraEnabled)
			mGrapple->GoToDestination(false);
	}
	else if (isFistSkillEnabled && FistComp && FistComp->IsTargetingFist)
	{
		if (isSightCameraEnabled)
			FistComp->GoToDestination();
	}
}

void AGolemProjectCharacter::TurnAtRate(float Rate)
{
	if (GetWorld())
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AGolemProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if (GetWorld())
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AGolemProjectCharacter::ChangeCameraPressed()
{
	if (PushingComponent && PushingComponent->GetIsPushingObject())
	{
		return;
	}
	if (sightCamera && pc)
	{
		if (!isSightCameraEnabled)
		{

			if ((isGrappleSkillEnabled && mGrapple && mGrapple->IsTargetingGrapple) || (isFistSkillEnabled && FistComp && FistComp->IsTargetingFist))
			{
				pc->SetViewTargetWithBlend(sightCamera->GetChildActor(), 0.25f);
				isSightCameraEnabled = true;
				if (GetCharacterMovement())
				{
					GetCharacterMovement()->bOrientRotationToMovement = false;
				}
				IsInteractingOrAiming = true;
			}

		}
	}
}

void AGolemProjectCharacter::ChangeCameraReleased()
{
	if (PushingComponent && PushingComponent->GetIsPushingObject())
	{
		return;
	}
	if (sightCamera && pc)
	{
		if (isSightCameraEnabled)
		{
			IsInteractingOrAiming = false;
			isSightCameraEnabled = false;
			if (GetCharacterMovement())
			{
				GetCharacterMovement()->bOrientRotationToMovement = true;
			}
			pc->SetViewTargetWithBlend(this, 0.25f);
		}
	}
}

void AGolemProjectCharacter::MoveForward(float Value)
{
	if (PushingComponent && PushingComponent->GetIsStartingPushingObject() || (mGrapple && mGrapple->GetFiring() && !mGrapple->GetSwingPhysics()))
	{
		return;
	}
	m_valueForward = Value;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		if (PushingComponent == nullptr || !PushingComponent->GetIsPushingObject())
		{
			if (mGrapple && FollowCamera && mGrapple->GetSwingPhysics())
			{
				mGrapple->GetSwingPhysics()->AddForceMovement(FollowCamera->GetForwardVector() * m_valueForward);
			}
			else
			{
				if (mGrapple && mGrapple->IsTargetingGrapple && (isSightCameraEnabled || mGrapple->GetProjectile()) && GetCharacterMovement() && !GetCharacterMovement()->IsFalling())
				{
					Direction = mGrapple->GetDirection();
				}
				else if (FistComp && FistComp->IsTargetingFist && isSightCameraEnabled)
				{
					Direction = FistComp->GetDirection();
				}
			}
		}
		else
		{
			if (PushingComponent == nullptr || (PushingComponent->GetIsPushingObject(false) &&
				(pushedObjectIsCollidingForward && Value > 0 || pushedObjectIsCollidingBackward && Value < 0)))
			{
				Direction = FVector::ZeroVector;
			}
			else
			{
				Direction = PushingComponent->GetPushingDirection();
			}
		}
		Direction = Direction.GetSafeNormal();
		AddMovementInput(Direction, Value);
	}
}

void AGolemProjectCharacter::MoveRight(float Value)
{
	if (PushingComponent && PushingComponent->GetIsPushingObject() || (mGrapple && mGrapple->GetFiring() && !mGrapple->GetSwingPhysics()))
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

		if (mGrapple && mGrapple->GetSwingPhysics() && FollowCamera)
		{
			mGrapple->GetSwingPhysics()->AddForceMovement(FollowCamera->GetRightVector() * m_valueRight);
		}
		else
		{
			// add movement in that direction
			if (isSightCameraEnabled && mGrapple &&
				(mGrapple->IsTargetingGrapple || mGrapple->GetProjectile()) && GetCharacterMovement() && !GetCharacterMovement()->IsFalling())
			{
				Direction = mGrapple->GetDirection();

				float X = Direction.X;
				float Y = Direction.Y;
				float Z = Direction.Z;

				Direction.X = -Y;
				Direction.Y = X;
			}
			else if (isSightCameraEnabled && FistComp && FistComp->IsTargetingFist)
			{
				Direction = FistComp->GetDirection();

				float X = Direction.X;
				float Y = Direction.Y;
				float Z = Direction.Z;

				Direction.X = -Y;
				Direction.Y = X;
			}
			Direction = Direction.GetSafeNormal();
			AddMovementInput(Direction, Value);
		}
	}
}

void AGolemProjectCharacter::ResetFriction()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GroundFriction = initialGroundFriction;
	}
}


void AGolemProjectCharacter::SetUpBlockOffsetPositon()
{
	if (actorToInteract && PushingComponent)
	{
		PushingComponent->SetBlockOffsetPosition(actorToInteract->GetActorLocation() - GetActorLocation());
		startPushingZ = GetActorLocation().Z;
		offsetLeftHand = leftHandPosition - GetActorLocation();
		offsetRightHand = rightHandPosition - GetActorLocation();
	}
}

bool AGolemProjectCharacter::PushBloc(FVector pushingDirection, FVector pushingPosition, FRotator pushingRotation)
{
	FVector tempPos = pushingPosition;
	if (GetWorld() && !GetWorld()->FindTeleportSpot(this, tempPos, pushingRotation) || tempPos != pushingPosition)
	{
		return false;
	}
	if (isSightCameraEnabled)
	{
		ChangeCameraReleased();
	}
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	if (PushingComponent)
	{
		PushingComponent->PushBloc(pushingDirection, pushingPosition, pushingRotation);
		PushingComponent->SetBlock(Cast<APushableBloc>(actorToInteract));
	}
	IsInteractingOrAiming = true;
	return true;
}


void AGolemProjectCharacter::StopPushBloc()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	if (PushingComponent)
	{
		PushingComponent->StopPushBloc();
	}
	pushedObjectIsCollidingForward = false;
	pushedObjectIsCollidingBackward = false;
	IsInteractingOrAiming = false;
}

void AGolemProjectCharacter::InflictDamage(int _damage)
{
	if (HealthComponent != nullptr)
		HealthComponent->InflictDamage(_damage);
}

//WIP DO NOT TOUCH
void AGolemProjectCharacter::ActivateDeath(bool _activate)
{
	/*if (_activate)
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetSimulatePhysics(true);
	}
	else
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetAllBodiesSimulatePhysics(false);
		GetMesh()->ResetAllBodiesSimulatePhysics();
		GetMesh()->RecreatePhysicsState();
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}*/
}

void AGolemProjectCharacter::ResetMeshOnRightPlace()
{
	//GetMesh()->SetupAttachment(GetCapsuleComponent());
	//GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -97.0f));
	//GetMesh()->SetRelativeRotation(GetCapsuleComponent()->GetComponentRotation());
}
