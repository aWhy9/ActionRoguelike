// Fill out your copyright notice in the Description page of Project Settings.


#include "RLPowerUp.h"

#include "NiagaraFunctionLibrary.h"
#include "RLAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ARLPowerUp::ARLPowerUp()
{ 	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(RootComponent);

	bIsActorHidden = false;

	RespawnTimerDelay = 2.0f;

	SetReplicates(true);

}

// Called when the game starts or when spawned
void ARLPowerUp::BeginPlay()
{
	Super::BeginPlay();

	
}



// Implementation of this actor's interact function from InteractionComponent
void ARLPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	if (bCanInteract)
	{
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ARLPowerUp::Respawn, RespawnTimerDelay);
	
		ToggleActorVisibility();
	}
	
	//IRGameplayInterface::Interact_Implementation(InstigatorPawn);
}

// Toggles Actor visibility and collision
void ARLPowerUp::ToggleActorVisibility()
{
	if (!bIsActorHidden)
	{
		bIsActorHidden = true;			
	}
	else
	{
		bIsActorHidden = false;		
	}
	
}

// Handles "respawning" / toggles visibility of the actor via the TimerHandle 
void ARLPowerUp::Respawn()
{
	GetWorldTimerManager().ClearTimer(RespawnTimerHandle);
	ToggleActorVisibility();
}

void ARLPowerUp::OnRep_IsActorHidden()
{
	if (!bIsActorHidden)
	{		
		SetActorHiddenInGame(true);		
		SetActorEnableCollision(false);
		
	}
	else
	{		
		SetActorHiddenInGame(false);		
		SetActorEnableCollision(true);
		
	}
}

void ARLPowerUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARLPowerUp, bIsActorHidden);
}
