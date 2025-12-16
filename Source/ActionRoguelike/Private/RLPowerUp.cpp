// Fill out your copyright notice in the Description page of Project Settings.


#include "RLPowerUp.h"

#include "Components/SphereComponent.h"

// Sets default values
ARLPowerUp::ARLPowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(RootComponent);

	bIsActorHidden = false;

	RespawnTimerDelay = 5.0f;

}

// Called when the game starts or when spawned
void ARLPowerUp::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ARLPowerUp::Respawn, RespawnTimerDelay);
}

// Called every frame
void ARLPowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARLPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	IRGameplayInterface::Interact_Implementation(InstigatorPawn);

	ShowActor();
	
}

void ARLPowerUp::ShowActor()
{
	if (!bIsActorHidden)
	{
		SetActorHiddenInGame(true);
		bIsActorHidden = true;
		//SetActorEnableCollision(false);
		
	}
	else
	{
		SetActorHiddenInGame(false);
		bIsActorHidden = false;
		//SetActorEnableCollision(true);
		
	}
	
}


//Currently only respawns once, need to fix these functions sets to make it work properly
void ARLPowerUp::Respawn()
{
	//GetWorldTimerManager().
	ShowActor();
}


