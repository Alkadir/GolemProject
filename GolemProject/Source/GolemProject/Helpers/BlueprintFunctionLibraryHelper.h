// Golem Project - Créajeux 2020

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintFunctionLibraryHelper.generated.h"

/**
 * 
 */
UCLASS()
class GOLEMPROJECT_API UBlueprintFunctionLibraryHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public :
	/** return true if target is already on screen and false otherwise
	*@param _targetLocation -> location of the object in world
	*@param _edgePercent how close to the edge of screen 0 center 1 on the edge (0.9 is cool)
	*@param _screenPosition position on screen of the target*/
	UFUNCTION(BlueprintCallable, Category = "HUD", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext))
	static bool FindScreenEdgeLocation(UObject* WorldContextObject, const FVector& _targetLocation, const float _edgePercent, float& _rotation, FVector2D& _screenPosition);
};
