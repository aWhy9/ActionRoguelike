// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RAction.h"

#include "RLAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, URLAttributeComponent*, OwningComponent, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API URLAttributeComponent : public UActorComponent
{
	GENERATED_BODY()


protected:

	// EditAnywhere - Edit in BP editor and per instance in level
	// VisibleAnywhere - 'read' only in editor and level (Use for components)
	// EditDefaultsAnywhere - hide variable per instance, edit in BP editor only
	// VisibleDefaultsOnly - 'read' only access for variable, only in BP editor (uncommon)
	// EditInstanceOnly - allow only editing of instance (eg. when placed in level)
	// --
	// BlueprintReadOnly - read-only in the Blueprint scripting (doesn't affect details panel)
	// BlueprintReadWrite - read-write access in Blueprints
	// --
	// Category = "" - Display only for details panel and blueprint context menu
	// --
	// meta = (Displayname = "") - Change Display name in editor
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float MaxRage;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static URLAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (Displayname = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	
	// Sets default values for this component's properties
	URLAttributeComponent();

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	
public:

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);
	
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRageChange(float Delta);
	
	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(Blueprintable)
	bool IsFullHealth() const;
};
