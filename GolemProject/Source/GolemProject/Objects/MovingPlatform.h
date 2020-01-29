// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Activable.h"
#include "MovingPlatform.generated.h"

UENUM(BlueprintType)
enum class EMovingDirection : uint8
{
	Forward		UMETA(DisplayName = "Forward"),
	Backward	UMETA(DisplayName = "Backward"),
	None		UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class EMovingPlatformType : uint8
{
	PingPong	UMETA(DisplayName = "PingPong"),
	Once		UMETA(DisplayName = "Once"),
	Loop		UMETA(DisplayName = "Loop"),
	OnceLoop	UMETA(DisplayName = "OnceLoop")
};

UENUM(BlueprintType)
enum class EMovingType : uint8
{
	Line		UMETA(DisplayName = "Line"),
	Curve		UMETA(DisplayName = "Curve")
};

UCLASS()
class GOLEMPROJECT_API AMovingPlatform : public AActor, public IActivable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovingPlatform(const FObjectInitializer& OI);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		bool isActivate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		EMovingPlatformType platformType = EMovingPlatformType::PingPong;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		EMovingDirection direction = EMovingDirection::Forward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		int startIndexCheckpoint = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		TArray<float> speeds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		TArray<float> waitTimes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		bool isStair = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		bool alwaysActive = false;

	int currentIndex;
	int nextIndex;
	int dir;
	float waitTime;
	float timerLerp;
	FVector velocity;
	bool isPause;


	UPROPERTY(EditDefaultsOnly)
		class USceneComponent* pathParent;
	UPROPERTY(EditDefaultsOnly)
		USceneComponent* path1;

	TArray<FVector> worldCheckpoint;

	void Init();
	void MoveLine(float dt);
	void SetNextIndex();

public:

	UFUNCTION(BlueprintCallable, Category = "Platform")
		const FVector GetPlatformVelocity()const { return velocity; }

	UFUNCTION(BlueprintCallable, Category = "Platform")
		const EMovingDirection GetDirection()const { return direction; }

	UFUNCTION(BlueprintCallable, Category = "Platform")
		const EMovingPlatformType GetPlatformType()const { return platformType; }

		const bool IsActivate() const { return isActivate; }
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Platform")
		void Pause() { isPause = true; }
	UFUNCTION(BlueprintCallable, Category = "Platform")
		void UnPause() { isPause = false; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Activable")
		const bool Activate(AActor* caller);
	virtual const bool Activate_Implementation(AActor* caller) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Activable")
		bool Desactivate(AActor* caller) const;
	virtual const bool Desactivate_Implementation(AActor* caller) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Activable")
		bool Switch(AActor* caller);
	virtual const bool Switch_Implementation(AActor* caller) override;


};
