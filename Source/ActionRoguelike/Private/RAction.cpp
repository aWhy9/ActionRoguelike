// Fill out your copyright notice in the Description page of Project Settings.


#include "RAction.h"
#include "RActionComponent.h"


bool URAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	URActionComponent* ActionComp = GetOwningComponent();
	
	if (ActionComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;	
	}
	return true;
}

void URAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Running: %s"), *GetNameSafe(this));

	URActionComponent* ActionComp = GetOwningComponent();

	ActionComp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;	
}

void URAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);
	
	URActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* URAction::GetWorld() const
{
	//Outer is set when creating action via NewObject<T>
	UActorComponent* ActorComp = Cast<UActorComponent>(GetOuter());
	if (ActorComp)
	{
		return ActorComp->GetWorld();
	}
	return nullptr;
}

float URAction::GetAbilityCost()
{
	return AbilityCost;
}

URActionComponent* URAction::GetOwningComponent() const
{
	return Cast<URActionComponent>(GetOuter());
}

bool URAction::IsRunning() const
{
	return bIsRunning;
}
