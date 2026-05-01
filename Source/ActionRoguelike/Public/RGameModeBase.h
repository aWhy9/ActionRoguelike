// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "RGameModeBase.generated.h"



class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class URSaveGame;
class UDataTable;
class URMonsterData;

/* Datatable Row for spawning monsters in Game mode */
USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;
	
	//TSubclassOf<AActor> MonsterClass;

	// Relative chance to pick this monster
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	// Points requires by Gamemode to spawn this unit
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	// Amount of credits awarded to killer of this unit
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;
};



/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	FString SlotName;
	
	UPROPERTY()
	URSaveGame* CurrentSaveGame;

	// All available monsters
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery; 

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;
	
	FTimerHandle TimeHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	// UPROPERTY(EditDefaultsOnly, Category = "AI")
	// TSubclassOf<AActor> MinionClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 NrOfAliveBots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 BotCoinValue = 5;	
	
	UFUNCTION()
	void SpawnBotTimerElapsed();
	
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	
	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);
	
	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
	
public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	ARGameModeBase();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void StartPlay() override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();
	
};
