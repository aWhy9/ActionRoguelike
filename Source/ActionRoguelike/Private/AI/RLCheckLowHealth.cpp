// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RLCheckLowHealth.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RLAttributeComponent.h"

URLCheckLowHealth::URLCheckLowHealth()
{
	LowHealthFraction = 0.6f;
}


void URLCheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check AI's AttributeComponent for current health is below threshold for concern

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (ensure(BlackboardComponent))
	{
		APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();		
		if (ensure(AIPawn))
		{
			URLAttributeComponent* AttributeComponent = Cast<URLAttributeComponent>(AIPawn->GetComponentByClass(URLAttributeComponent::StaticClass()));
			if (ensure(AttributeComponent))
			{
				float CurrentHealth = AttributeComponent->GetHealth();
				float MaxHealth = AttributeComponent->GetMaxHealth();
				bool bIsLowHealth = (CurrentHealth / MaxHealth) <= LowHealthFraction;					
							
				BlackboardComponent->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsLowHealth);					
			}				
		}			
	}
}
