// Fill out your copyright notice in the Description page of Project Settings.


#include "RActionComponent.h"

#include "RAction.h"


URActionComponent::URActionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;


}



void URActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<URAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
	
}


void URActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString  DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);	
}

void URActionComponent::AddAction(AActor* Instigator, TSubclassOf<URAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	URAction* NewAction = NewObject<URAction>(this, ActionClass);
	if (ensure(NewAction))
	{
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
				Action->StopAction(Instigator);
				return true;
			}
		}
	}	
	return false;
}
