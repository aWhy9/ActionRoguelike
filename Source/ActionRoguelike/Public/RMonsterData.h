// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RMonsterData.generated.h"

class UTexture2D;
class URAction;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TSubclassOf<AActor> MonsterClass;

	// Actions/buffs to gran this monster
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TArray<TSubclassOf<URAction>> Actions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Monsters", GetFName());
	}
};
