// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "RAction.generated.h"

class UWorld;
class UActorComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	bool bIsRunning;
	
	UPROPERTY()
	AActor* Instigator;
};

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API URAction : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(Replicated)
	URActionComponent* ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	URActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	float AbilityCost;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;
	//bool bIsRunning;

	UFUNCTION()
	void OnRep_RepData();
	
public:

	void Initialize(URActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category= "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);
	
	// Action Nicknname to start/stop without a reference to the object
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	float GetAbilityCost();
	
	bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
