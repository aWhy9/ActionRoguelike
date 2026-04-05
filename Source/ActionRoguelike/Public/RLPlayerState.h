// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RLPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARLPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Credits")
	int32 Credits = 0;

	

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 RemoveCredits(int32 amount);
	
	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 AddCredits(int32 amount);
	
};
