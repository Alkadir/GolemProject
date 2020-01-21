// Fill out your copyright notice in the Description page of Project Settings.


#include "HelperLibrary.h"
#include "Engine/Engine.h"
#include "Containers/Array.h"

void HelperLibrary::Print(const FString& _message, const float& _time, const FColor _color)
{
	GEngine->AddOnScreenDebugMessage(-1, _time, _color, _message);
}

void HelperLibrary::SortActorsByDistanceTo(TArray<AActor*> _inOutActorArray, AActor* _character)
{
	if (_character != nullptr && _inOutActorArray.Num() > 0)
	{
		bool isSorted = false;
		float distance = _inOutActorArray[0]->GetDistanceTo(_character);
		/*HelperLibrary::ActorToCalculateDistanceWith = _character;
		_inOutActorArray.Sort(SortByDistance);*/
		while (!isSorted)
		{
			isSorted = true;
			for (int i = 0; i < _inOutActorArray.Num() - 1; ++i)
			{
				if (_inOutActorArray[i]->GetDistanceTo(_character) > _inOutActorArray[i + 1]->GetDistanceTo(_character))
				{
					_inOutActorArray.Swap(i, i + 1);
					isSorted = false;
				}
			}
		}
	}
}
