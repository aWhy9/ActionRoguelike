// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RLAICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ARLAICharacter::ARLAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
}

void ARLAICharacter::PostInitializeComponents()
{

	Super::PostInitializeComponents();
	
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ARLAICharacter::OnPawnSeen);
}

void ARLAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
		if (ensureMsgf(BBComp, TEXT("No Blackboard Component, please ensure BB and Behavior Tree is set.")))
		{
			BBComp->SetValueAsObject("TargetActor", Pawn);
			DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::Magenta, 4.0f, true);
		}
		else
		{
			DrawDebugString(GetWorld(), GetActorLocation(), "NO BLACKBOARD COMPONENT, I DON'T KNOW WHAT TO DO.", nullptr, FColor::Red, 4.0f, true);
		}
	}
}

