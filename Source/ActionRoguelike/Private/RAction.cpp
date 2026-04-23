// Fill out your copyright notice in the Description page of Project Settings.


#include "RAction.h"
#include "RActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

void URAction::Initialize(URActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool URAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	URActionComponent* Comp = GetOwningComponent();
	
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;	
	}
	return true;
}

void URAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Warning, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
	
	URActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;	
}

void URAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Warning, TEXT("Stopped: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	
	//ensureAlways(bIsRunning);
	
	URActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* URAction::GetWorld() const
{
	//Outer is set when creating action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

float URAction::GetAbilityCost()
{
	return AbilityCost;
}

URActionComponent* URAction::GetOwningComponent() const
{	
	return ActionComp;
}

void URAction::OnRep_IsRunning()
{
	if (bIsRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}


bool URAction::IsRunning() const
{
	return bIsRunning;
}

void URAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URAction, bIsRunning);
	DOREPLIFETIME(URAction, ActionComp);
	
}