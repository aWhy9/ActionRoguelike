// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RLDoorBase.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ARLDoorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARLDoorBase();
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOpen;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DoorSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector InitialLocation;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void ActivateDoor();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
