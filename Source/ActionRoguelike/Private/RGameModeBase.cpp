// Fill out your copyright notice in the Description page of Project Settings.


#include "RGameModeBase.h"

#include "EngineUtils.h"
#include "RLAttributeComponent.h"
#include "AI/RLAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

ARGameModeBase::ARGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ARGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn more bots
	// Actual amount of bots and whether it's allowed to spawn determined by spawn logic later in chain

	GetWorldTimerManager().SetTimer(TimeHandle_SpawnBots, this, &ARGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}


void ARGameModeBase::SpawnBotTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARGameModeBase::OnQueryCompleted);
	}
	
}

void ARGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed"));
		return;
	}

	NrOfAliveBots = 0;
	for (TActorIterator<ARLAICharacter> It(GetWorld()); It; ++ It)
	{
		ARLAICharacter* Bot = *It;

		URLAttributeComponent* AttributeComponent = Cast<URLAttributeComponent>(Bot->GetComponentByClass(URLAttributeComponent::StaticClass()));
		if (AttributeComponent && AttributeComponent->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	float MaxBotCount = 10.0f;
	
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if (NrOfAliveBots >= MaxBotCount)
	{
		return;
	}




	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}