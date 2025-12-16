// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "RLPowerUp.generated.h"

class UStaticMeshComponent;
class USphereComponent;

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

	UPROPERTY(VisibleAnywhere)
	bool bIsActorHidden;

	UPROPERTY(VisibleAnywhere)
	float RespawnTimerDelay;
	
	UFUNCTION(BlueprintCallable)
	void Interact_Implementation(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable)
	void ShowActor();

	void Respawn();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
