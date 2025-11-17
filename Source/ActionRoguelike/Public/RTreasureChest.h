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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsOpen;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
