// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RAnimInstance.generated.h"

class URActionComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:

	// Is Pawn stunned based on GameplayTag Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	URActionComponent* ActionComp;
	
	void NativeInitializeAnimation() override;

	void NativeUpdateAnimation(float DeltaSeconds) override;
};
