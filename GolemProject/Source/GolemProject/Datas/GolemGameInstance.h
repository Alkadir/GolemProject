// Golem Project - Créajeux 2020

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GolemGameInstance.generated.h"

/**
 *
 */
UCLASS()
class GOLEMPROJECT_API UGolemGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
		int indexStartPlayer = 0;
	UPROPERTY(BlueprintReadWrite)
		bool displayHelpHUD = true;

	UFUNCTION(BlueprintCallable)
		void SwitchDisplayHelpHud()
	{
		displayHelpHUD = !displayHelpHUD;
	}
};
