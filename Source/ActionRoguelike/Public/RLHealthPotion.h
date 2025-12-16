// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLPowerUp.h"
#include "RLHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARLHealthPotion : public ARLPowerUp
{
	GENERATED_BODY()
	


public:

ARLHealthPotion();

UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
float HealAmount;	

UFUNCTION(BlueprintCallable)
void Heal();
	
};