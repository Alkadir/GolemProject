// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player/DashComponent.h"
#include "Interfaces/Interactable.h"
#include "GolemProjectCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartMoving);
UCLASS(config = Game)
class AGolemProjectCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	bool isGrappleSkillEnabled = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	bool isFistSkillEnabled = false;

	UPROPERTY()
	bool isSightCameraEnabled = false;

	float m_valueForward;

	float m_valueRight;

	bool WantToAim;

	UPROPERTY()
	class UUserWidget* currentSightWidget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grapple Hook", meta = (AllowPrivateAccess = "true"))
	class UGrappleComponent* mGrapple;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FistComp, meta = (AllowPrivateAccess = "true"))
	class UFistComponent* FistComp;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* customCapsule;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pushing, meta = (AllowPrivateAccess = "true"))
	class UPushingComponent* PushingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pushing, meta = (AllowPrivateAccess = "true"))
	class URaycastingComponent* RaycastingComponent;

	class APlayerController* pc;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pushing, meta = (AllowPrivateAccess = "true"))
	class UWallMechanicalComponent* WallMechanicalComponent;

	UPROPERTY()
	class UChildActorComponent* sightCamera;

	UPROPERTY()
	class UChildActorComponent* sightCameraL;

	float initialGroundFriction;

	bool HasPressedAiming;

	bool HasAlreadyMove;

protected:

	UPROPERTY(BlueprintReadWrite)
	AActor* actorToInteract;
	UPROPERTY(BlueprintReadWrite)
	bool pushedObjectIsCollidingForward;
	UPROPERTY(BlueprintReadWrite)
	bool pushedObjectIsCollidingBackward;

	float startPushingZ;

	UPROPERTY(BlueprintReadWrite)
	FVector rightHandPosition;
	FVector offsetRightHand;
	UPROPERTY(BlueprintReadWrite)
	FVector leftHandPosition;
	FVector offsetLeftHand;


	virtual void BeginPlay() override;
	virtual void Tick(float _deltaTime) override;

	//virtual void Tick(float _deltaTime) override;

	void Fire();

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

	void ChangeCameraPressed();

	void ChangeCameraReleased();

	UPROPERTY(EditAnywhere)
	UDashComponent* dashComponent;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Jump() override;

	void Dash();

	void UseAssistedGrapple();

	UFUNCTION()
	void SetUpBlockOffsetPositon();

	UFUNCTION()
	void AimAtEndOfWallJump();

	void SwitchArm();

public:
	AGolemProjectCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hud")
	TSubclassOf<class UUserWidget>  sightHudClass;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UFUNCTION(BlueprintCallable, Category = "Hud")
	FORCEINLINE bool& GetSightCameraEnabled() { return isSightCameraEnabled; };

	FORCEINLINE bool& IsGrappleSkillEnabled() { return isGrappleSkillEnabled; };

	FORCEINLINE bool& IsFistSkillEnabled() { return isFistSkillEnabled; };

	FORCEINLINE void SetGrappleSkillEnabled(bool _enable) { isGrappleSkillEnabled = _enable; };

	FORCEINLINE void SetFistSkillEnabled(bool _enable) { isFistSkillEnabled = _enable; };

	void ResetFriction();
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	FORCEINLINE class URaycastingComponent* GetRaycastingComponent() const { return RaycastingComponent; }

	FORCEINLINE class UCapsuleComponent* GetCustomCapsuleComponent() const { return customCapsule; }

	FORCEINLINE class UDashComponent* GetDashComponent() const { return dashComponent; }

	UFUNCTION(BlueprintCallable)
	FVector GetVirtualRightHandPosition();

	UFUNCTION(BlueprintCallable)
	FVector GetVirtualLeftHandPosition();

	UFUNCTION(BlueprintCallable, Category = "Dash")
	FORCEINLINE bool IsDashing() { return dashComponent->IsDashing(); };

	UFUNCTION(BlueprintCallable, Category = "IK")
	void SetRightHandPosition(FVector newPos) { rightHandPosition = newPos; }
	UFUNCTION(BlueprintCallable, Category = "IK")
	void SetLeftHandPosition(FVector newPos) { leftHandPosition = newPos; }


	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Dash Action Event"), Category = Events)
	void DashEvent();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Fire Action Event"), Category = Events)
	void FireEvent();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Grappling Action Event"), Category = Events)
	void GrapplingFireEvent();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "End Grappling Action Event"), Category = Events)
	void EndGrapplingEvent();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = " Start Swing Action Event"), Category = Events)
	void StartSwingEvent();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Starting Attraction Action Event"), Category = Events)
	void StartingAttractionEvent();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Coming Back Action Event"), Category = Events)
	void ComingBackEvent();

	bool PushBloc(FVector pushingDirection, FVector pushingPosition, FRotator pushingRotation);

	void StopPushBloc();

	void ChangeToGrapple();

	void ChangeToFist();

	void InflictDamage(int _damage);

	bool IsCharacterDead();

	void ActivateDeath(bool _activate);

	void ResetMeshOnRightPlace();

	UPROPERTY(BlueprintReadOnly)
	bool IsInteracting;

	UPROPERTY(BlueprintReadOnly)
	bool IsAiming;

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateTargetGrapple(class AActor* _target);

	UFUNCTION(BlueprintImplementableEvent)
	void DeactivateTargetGrapple();

	UFUNCTION(BlueprintImplementableEvent)
	void Event_Death();

	UFUNCTION(BlueprintCallable)
	bool IsCharacterSwinging(); 

	UPROPERTY(EditAnyWhere, BlueprintReadWrite ,Category = "Debug")
	bool showCursor = false;

	UPROPERTY(BlueprintAssignable)
	FStartMoving OnStartMoving;
};
