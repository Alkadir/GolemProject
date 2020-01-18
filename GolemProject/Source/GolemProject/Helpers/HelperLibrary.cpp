// Fill out your copyright notice in the Description page of Project Settings.


#include "HelperLibrary.h"
#include "Engine/Engine.h"

void HelperLibrary::Print(const float& _time, const FString& _message)
{
	GEngine->AddOnScreenDebugMessage(0, _time, FColor::Blue, _message);
}
