// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player/DashComponent.h"
#include "Interfaces/Interactable.h"
#include "GolemProjectCharacter.generated.h"


UCLASS(config = Game)
class AGolemProjectCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnyWhere, Category = "Debug")
		bool showCursor = false;

	UPROPERTY()
		bool isSightCameraEnabled = false;

	float m_valueForward;

	float m_valueRight;

	UPROPERTY()
		class UUserWidget* currentSightWidget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grapple Hook", meta = (AllowPrivateAccess = "true"))
		class UGrappleComponent* mGrapple;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FistComp, meta = (AllowPrivateAccess = "true"))
		class UFistComponent* FistComp;

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

	class APlayerController* pc;

	UPROPERTY()
		class UChildActorComponent* sightCamera;

	UPROPERTY()
		class UChildActorComponent* sightCameraL;

	float initialGroundFriction;

protected:

	UPROPERTY(BlueprintReadWrite)
		AActor* actorToInteract;
	UPROPERTY(BlueprintReadWrite)
		bool pushedObjectIsCollidingForward;
	UPROPERTY(BlueprintReadWrite)
		bool pushedObjectIsCollidingBackward;
	float startPushingZ;

	virtual void BeginPlay() override;
	virtual void Tick(float _deltaTime) override;

	//To do
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

	void ChangeCamera();




	UPROPERTY(EditAnywhere)
		UDashComponent* dashComponent;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Jump() override;

	void Dash();

	void UseAssistedGrapple();

	void ChangeToGrapple();

	void ChangeToFist();

	UFUNCTION()
		void SetUpBlockOffsetPositon();

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

	void ResetFriction();
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = "Dash")
		FORCEINLINE bool IsDashing() { return dashComponent->IsDashing(); };

	bool PushBloc(FVector pushingDirection, FVector pushingPosition, FRotator pushingRotation);

	void StopPushBloc();

	void InflictDamage(int _damage);

	void ActivateDeath(bool _activate);

	UPROPERTY(BlueprintReadOnly)
	bool IsInteractingOrAiming;
};
