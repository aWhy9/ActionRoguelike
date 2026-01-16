// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RLBTTask_Heal.h"

#include "AIController.h"
#include "RLAttributeComponent.h"
#include "GameFramework/Character.h"

URLBTTask_Heal::URLBTTask_Heal()
{
	HealAmount = 25;
}

EBTNodeResult::Type URLBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	URLAttributeComponent* AttributeComponent = URLAttributeComponent::GetAttributes(MyPawn);
	if (ensure(AttributeComponent))
	{		
		AttributeComponent->ApplyHealthChange(MyPawn, HealAmount);
	}		
	
	return EBTNodeResult::Succeeded;
}
