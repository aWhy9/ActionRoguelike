// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerController.h"

void ARPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}
