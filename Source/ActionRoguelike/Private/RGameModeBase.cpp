// Fill out your copyright notice in the Description page of Project Settings.


#include "RGameModeBase.h"

#include "EngineUtils.h"
#include "RLAttributeComponent.h"
#include "AI/RLAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "DrawDebugHelpers.h"
#include "RLPlayerState.h"
#include "RoguelikeCharacter.h"
#include "RSaveGame.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("rl.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ARGameModeBase::ARGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	NrOfAliveBots = 0;

	PlayerStateClass = ARLPlayerState::StaticClass();

	SlotName = "SaveGame_01";
}

void ARGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
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
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via 'CVArSpawnBots'."));
		return;
	}
	
	NrOfAliveBots = 0;
	for (TActorIterator<ARLAICharacter> It(GetWorld()); It; ++ It)
	{
		ARLAICharacter* Bot = *It;

		URLAttributeComponent* AttributeComponent = URLAttributeComponent::GetAttributes(Bot);
		//if (ensure(AttributeComponent && AttributeComponent->IsAlive()))
		if (AttributeComponent && AttributeComponent->IsAlive())
		{
			NrOfAliveBots++;
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), NrOfAliveBots);
	
	float MaxBotCount = 10.0f;	
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARGameModeBase::OnQueryCompleted);
	}
	
}

// Spawn AI Actor
void ARGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		// Track all the used spawn locations
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}


// Debug for killing all AI
void ARGameModeBase::KillAll()
{
	for (TActorIterator<ARLAICharacter> It(GetWorld()); It; ++ It)
	{
		ARLAICharacter* Bot = *It;
		
		UE_LOG(LogTemp, Warning, TEXT("AI TRACKED: %p"), Bot);
		
		URLAttributeComponent* AttributeComponent = URLAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComponent && AttributeComponent->IsAlive()))
		{
			AttributeComponent->Kill(this); // @fixme: pass in player? for kill credit
		}
	}
}

void ARGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{

	// Respawn Victim
	ARoguelikeCharacter* Player = Cast<ARoguelikeCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	// Award Credits to killer if they have a valid PlayerState
	ARoguelikeCharacter* KillerPlayer = Cast<ARoguelikeCharacter>(Killer);
	if (KillerPlayer)
	{
		ARLPlayerState* PS = KillerPlayer->GetPlayerState<ARLPlayerState>();
		if (PS)
		{
			PS->AddCredits(BotCoinValue);	
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim %s, Killer %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void ARGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);		
	}	
}

void ARGameModeBase::WriteSaveGame()
{
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ARGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<URSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame data."));
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame data."));
	}
	else
	{
		CurrentSaveGame =  Cast<URSaveGame>(UGameplayStatics::CreateSaveGameObject(URSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created new SaveGame data."));
	}
}