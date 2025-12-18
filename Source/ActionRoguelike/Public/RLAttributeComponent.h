// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RLAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, URLAttributeComponent*, OwningComponent, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API URLAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URLAttributeComponent();

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
public:
	
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta);

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float MaxHealth;


};
