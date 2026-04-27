// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RAction.h"
#include "RActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URActionEffect : public URAction
{
	GENERATED_BODY()
	
public:
	
	URActionEffect();
	
	void StartAction_Implementation(AActor* Instigator) override;	
	void StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION(BlueprintCallable)
	float GetTimeRemaining() const;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	// Time between ticks to apply effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;
	
	FTimerHandle DurationHandle;	
	FTimerHandle PeriodHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);
	
};
