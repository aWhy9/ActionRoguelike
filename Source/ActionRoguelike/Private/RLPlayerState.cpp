// Fill out your copyright notice in the Description page of Project Settings.


#include "RLPlayerState.h"

int32 ARLPlayerState::RemoveCredits(int32 amount)
{
	Credits -= amount;
	return 0;
}

int32 ARLPlayerState::AddCredits(int32 amount)
{
	Credits += amount;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Credits Added"));
	return 0;
}
