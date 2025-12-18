// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "RLPowerUp.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class ACTIONROGUELIKE_API ARLPowerUp : public AActor, public IRGameplayInterface
{
	GENERATED_BODY()

private:

	FTimerHandle RespawnTimerHandle;
	
public:	
	// Sets default values for this actor's properties
	ARLPowerUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNiagaraSystem* InteractVFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* InteractSound;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsActorHidden;

	UPROPERTY(VisibleAnywhere)
	bool bCanInteract;

	UPROPERTY(VisibleAnywhere)
	float RespawnTimerDelay;

public:
	
	UFUNCTION(BlueprintCallable)
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION(BlueprintCallable)
	void ToggleActorVisibility();

	void Respawn();

	// Called every frame	
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
