// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GolemProjectCharacter.generated.h"

UCLASS(config=Game)
class AGolemProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, Category = "Debug")
	bool showCursor = false;

	UPROPERTY()
	bool isSightCameraEnabled = false;

	UPROPERTY()
	class UUserWidget* currentSightWidget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grapple Hook", meta = (AllowPrivateAccess = "true"))
	class UGrappleComponent* mGrapple;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY()
	class UChildActorComponent* sightCamera;

	float initialGroundFriction;
public:
	AGolemProjectCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hud")
	TSubclassOf<class UUserWidget>  sightHudClass;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UFUNCTION(BlueprintCallable, Category = "Hud")
	FORCEINLINE bool& GetSightCameraEnabled() { return isSightCameraEnabled; };

	void ResetFriction();

protected:

	virtual void BeginPlay() override;

	void Fire();

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void ChangeCamera();

	float m_valueForward;

	float m_valueRight;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool isPushing = false;

	UPROPERTY(EditAnywhere)
	class UDashComponent* dashComponent;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Jump() override;

	void Dash();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
