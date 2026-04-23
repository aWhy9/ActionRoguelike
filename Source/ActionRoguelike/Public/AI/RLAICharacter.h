// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RLAICharacter.generated.h"

class UPawnSensingComponent;
class URLAttributeComponent;
class URActionComponent;
class UUserWidget;
class URLWorldUserWidget;
class AActor;

UCLASS()
class ACTIONROGUELIKE_API ARLAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARLAICharacter();

protected:

	
	URLWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URLAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URActionComponent* ActionComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	
	UPROPERTY(VisibleAnywhere)
	FName TimeToHitParamName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<URLWorldUserWidget> SpottedWidgetClass;

protected:
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComponent, float NewHealth, float Delta);

	UFUNCTION()
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION()
	AActor* GetTargetActor() const;
};
