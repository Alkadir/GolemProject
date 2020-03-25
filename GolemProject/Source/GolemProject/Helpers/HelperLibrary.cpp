// Fill out your copyright notice in the Description page of Project Settings.


#include "HelperLibrary.h"
#include "Engine/Engine.h"
#include "Containers/Array.h"
AActor* HelperLibrary::ActorToCalculateDistanceWith = nullptr;
void HelperLibrary::Print(const FString& _message, const float& _time, const FColor _color)
{
//#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, _time, _color, _message);
//#endif
}

void HelperLibrary::SortActorsByDistanceTo(TArray<AActor*>& _inOutActorArray, AActor* _character)
{
	if (_character != nullptr && _inOutActorArray.Num() > 0)
	{
		HelperLibrary::ActorToCalculateDistanceWith = _character;
		_inOutActorArray.Sort(SortByDistance);
	}
}
