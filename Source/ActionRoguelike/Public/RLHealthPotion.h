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
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float HealAmount;	

	UPROPERTY(EditAnywhere)
	int32 CreditCost = 5;
	
public:

	ARLHealthPotion();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
		
};