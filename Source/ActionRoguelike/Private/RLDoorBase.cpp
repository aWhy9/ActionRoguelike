// Fill out your copyright notice in the Description page of Project Settings.


#include "RLDoorBase.h"

// Sets default values
ARLDoorBase::ARLDoorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;
	
	bIsOpen = false;
	DoorSpeed = 1.0f;

	

}

// Called when the game starts or when spawned
void ARLDoorBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitialLocation = MeshComponent->GetRelativeLocation();
}

void ARLDoorBase::ActivateDoor()
{
	
	
	if (!bIsOpen)
	{
		MeshComponent->SetRelativeLocation(InitialLocation + (FVector(0,0,-295)));
		bIsOpen = true;
	}	
	else
	{
		MeshComponent->SetRelativeLocation(InitialLocation);
		bIsOpen = false;
	}
	
}

// Called every frame
void ARLDoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

