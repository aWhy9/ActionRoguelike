// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RLAICharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "RActionComponent.h"
#include "RLAttributeComponent.h"
#include "RLWorldUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ARLAICharacter::ARLAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AttributeComponent = CreateDefaultSubobject<URLAttributeComponent>("Attribute Component");

	ActionComp = CreateDefaultSubobject<URActionComponent>("Action Component");
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

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
			//UE_LOG(LogTemp, Warning, TEXT("Target Damaged me and Set to: %p"), InstigatorActor);
		}

		// Spawn Healthbar when damage is taken
		if (ActiveHealthBar == nullptr)
		{			
			ActiveHealthBar = CreateWidget<URLWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		// Hit Flash
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		
		// Died
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

			// Ensure no collision walls remain in world and it doesn't endlessly fall
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			
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
	//UE_LOG(LogTemp, Warning, TEXT("Target Seen and Set to: %p"), Pawn);
}


