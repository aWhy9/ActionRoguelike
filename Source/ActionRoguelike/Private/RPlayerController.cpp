// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerController.h"

#include "Blueprint/UserWidget.h"


void ARPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ARPlayerController::TogglePauseMenu);
}

void ARPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ARPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}

void ARPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		// Set to nullptr so garbage collection destroys it
		PauseMenuInstance = nullptr;
		bShowMouseCursor = false;

		SetInputMode(FInputModeGameOnly());
		return;
	}
	
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}
