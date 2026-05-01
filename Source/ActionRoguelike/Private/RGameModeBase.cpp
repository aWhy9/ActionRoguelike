// Fill out your copyright notice in the Description page of Project Settings.


#include "RGameModeBase.h"

#include "EngineUtils.h"
#include "RLAttributeComponent.h"
#include "AI/RLAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "DrawDebugHelpers.h"
#include "RActionComponent.h"
#include "RGameplayInterface.h"
#include "RLPlayerState.h"
#include "RMonsterData.h"
#include "RoguelikeCharacter.h"
#include "RSaveGame.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/AssetManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

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

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	if (SelectedSaveSlot.Len() > 0)
	{
		SlotName = SelectedSaveSlot; 
	}
	
	LoadSaveGame();
}

void ARGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn more bots
	// Actual amount of bots and whether it's allowed to spawn determined by spawn logic later in chain

	GetWorldTimerManager().SetTimer(TimeHandle_SpawnBots, this, &ARGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ARGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Calling before Super:: so we set variables before 'beginplayingstate' is called in PlayerController (which is where we instantiate UI)
	ARLPlayerState* PS = NewPlayer->GetPlayerState<ARLPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
	
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
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
		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);

			// Get Random enemy
			int32 RandomIndex = FMath::RandRange(0, Rows.Num()-1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* Manager = UAssetManager::GetIfValid();
			if (Manager)
			{

				LogOnScreen(this, "Loading monster...", FColor::Green);
				
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ARGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);				
				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}			
		}
		// Track all the used spawn locations
		//DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void ARGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	LogOnScreen(this, "Finished Loading.", FColor::Green);
	
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		URMonsterData* MonsterData = Cast<URMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));
		
		AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
		if (NewBot)
		{
			LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));
	
			// Grant Special Actions, buffs, etc
			URActionComponent* ActionComp = Cast<URActionComponent>(NewBot->GetComponentByClass(URActionComponent::StaticClass()));
			if (ActionComp)
			{
				for (TSubclassOf<URAction> ActionClass : MonsterData->Actions)
				{
					ActionComp->AddAction(NewBot, ActionClass);
				}
			}
		}
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
	// Iterate all player states, we don't have proper id to match yet (requires Steam orf EQS)
	for (int32 i = 0; i < GameState->PlayerArray.Num(); ++i)
	{
		ARLPlayerState* PS = Cast<ARLPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // Single Player only at this point
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	// Iterate the entire world of actors
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// Only interested in our gameplay actors
		if (!Actor->Implements<URGameplayInterface>())
		{
			continue;
		}
		
		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();
		
		// Pass the array to fill data from Actor
		FMemoryWriter MemWriter(ActorData.ByteData);
		
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		
		// Converts Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	
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

		// Iterate the entire world of actors
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			// Only interested in our gameplay actors
			if (!Actor->Implements<URGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);
					
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					// Find only variables with UPROPERTY(SaveGame)
					Ar.ArIsSaveGame = true;
		
					// Convert binary array back into actor's variables
					Actor->Serialize(Ar);

					IRGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame =  Cast<URSaveGame>(UGameplayStatics::CreateSaveGameObject(URSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created new SaveGame data."));
	}	
}