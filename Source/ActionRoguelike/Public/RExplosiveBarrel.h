// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RExplosiveBarrel.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USceneComponent;
class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API ARExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARExplosiveBarrel();


protected:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* RadialForceComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ImpulseRadius;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Explode();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
