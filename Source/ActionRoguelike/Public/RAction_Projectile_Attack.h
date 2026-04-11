// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RAction.h"
#include "RAction_Projectile_Attack.generated.h"

class UAnimMontage;
class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URAction_Projectile_Attack : public URAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;	

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNiagaraSystem* CastingVFX;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

protected:

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

	URAction_Projectile_Attack();
	
};
