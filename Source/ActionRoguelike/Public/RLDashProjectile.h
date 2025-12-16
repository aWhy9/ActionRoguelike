// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLProjectileBase.h"
#include "RLDashProjectile.generated.h"

/**
 * 
 */

class UNiagaraSystem;

UCLASS()
class ACTIONROGUELIKE_API ARLDashProjectile : public ARLProjectileBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundBase* ImpactSound;

	// Handle to cancel timer if we already hit something
	FTimerHandle TimeHandle_DelayedDetonate;

	// Base class using BlueprintNative|Event, we must override the _Implementation not the explode
	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	virtual void BeginPlay() override;

public:

	ARLDashProjectile();
	
};


