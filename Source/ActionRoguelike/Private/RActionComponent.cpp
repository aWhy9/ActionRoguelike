// Fill out your copyright notice in the Description page of Project Settings.


#include "RActionComponent.h"

#include "RAction.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_STANFORD);

URActionComponent::URActionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

}



void URActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Server only
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<URAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}	
}

void URActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Stop All actions remaining
	TArray<URAction*> ActionsCopy = Actions;
	for (URAction* Action: ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

void URActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString  DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// Draw All Actions
	for (URAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));		
			
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void URActionComponent::AddAction(AActor* Instigator, TSubclassOf<URAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	// Skip for clients
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction: [Class %s]"), *GetNameSafe(ActionClass));
		return;
	}
	
	URAction* NewAction = NewObject<URAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void URActionComponent::RemoveAction(URAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	Actions.Remove(ActionToRemove);
}

bool URActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);
	
	for (URAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FailedMsg);	
				continue;
			}
			
			// Is Client?
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
			// Bookmark for Unreal Insights
			TRACE_BOOKMARK(TEXT("Start Action::%s"), *GetNameSafe(Action));
			
			Action->StartAction(Instigator);
			return true;
		}
	}	
	return false;
}

bool URActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (URAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				// Is Client?
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}
				
				Action->StopAction(Instigator);
				return true;
			}
		}
	}	
	return false;
}

void URActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void URActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

void URActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URActionComponent, Actions);
	
}

bool URActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething =  Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (URAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}
