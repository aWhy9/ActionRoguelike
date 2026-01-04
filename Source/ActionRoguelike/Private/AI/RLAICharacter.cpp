// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RLAICharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "RLAttributeComponent.h"

// Sets default values
ARLAICharacter::ARLAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AttributeComponent = CreateDefaultSubobject<URLAttributeComponent>("Attribute Component");

	TimeToHitParamName = "TimeToHit";
}

void ARLAICharacter::PostInitializeComponents()
{

	Super::PostInitializeComponents();
	
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ARLAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ARLAICharacter::OnHealthChanged);
}

void ARLAICharacter::OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComponent, float NewHealth,
	float Delta)
{
	if (Delta < 0.0f)
	{

		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
			UE_LOG(LogTemp, Warning, TEXT("Target Damaged me and Set to: %p"), InstigatorActor);
		}
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		
		if (NewHealth <= 0.0f)
		{
			// Stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			
			// Ragdoll on death
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName(("Ragdoll"));
			
			// Set lifespan
			SetLifeSpan(10.0f);
		}
	}
}

void ARLAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (ensure(AIC))
	{		
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);	
	}	
}

void ARLAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	UE_LOG(LogTemp, Warning, TEXT("Target Seen and Set to: %p"), Pawn);
}


