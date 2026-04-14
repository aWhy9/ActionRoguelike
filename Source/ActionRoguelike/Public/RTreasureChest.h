// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "RTreasureChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ARTreasureChest : public AActor, public IRGameplayInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float TargetPitch;
	
public:	
	// Sets default values for this actor's properties
	ARTreasureChest();

	UFUNCTION(BlueprintCallable)
	void Interact_Implementation(APawn* InstigatorPawn);
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	// RepUsing is same as RepNotify but in C++
	UPROPERTY(ReplicatedUsing= "OnRep_LidOpened", VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsOpen;

	UFUNCTION()
	void OnRep_LidOpened();
};