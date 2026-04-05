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

	
	
}


void URActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void URActionComponent::AddAction(TSubclassOf<URAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	URAction* NewAction = NewObject<URAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool URActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (URAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
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
			Action->StopAction(Instigator);
			return true;
		}
	}
	
	return false;
}
