// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RLTargetDummy.generated.h"

class UStaticMeshComponent;
class URLAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ARLTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARLTargetDummy();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URLAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComponent, float NewHealth, float Delta);
	
};
