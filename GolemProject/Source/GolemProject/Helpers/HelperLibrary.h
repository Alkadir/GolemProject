// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
/**
 *
 */
class GOLEMPROJECT_API HelperLibrary
{
public:
	template <class T>
	static T* GetComponentByName(const AActor* _actor, const FString& _name)
	{
		TArray<UActorComponent*> objts;
		_actor->GetComponents<UActorComponent>(objts);

		for (int i = 0; i < objts.Num(); i++)
		{
			UActorComponent* component = objts[i];

			if (component && component->GetName() == _name)
				return Cast<T>(component);
		}
		return nullptr;
	};

	static void Print(const float& _time, const FString& _message);
};
