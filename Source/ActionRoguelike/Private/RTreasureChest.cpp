// Fill out your copyright notice in the Description page of Project Settings.


#include "RTreasureChest.h"


// Sets default values
ARTreasureChest::ARTreasureChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("Lid Mesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;
	bIsOpen = false;
	
}

void ARTreasureChest::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!bIsOpen)
	{
		LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
		bIsOpen = true;
	}
	else
	{
		LidMesh->SetRelativeRotation(FRotator(0, 0, 0));
		bIsOpen = false;
	}
	
	
}

// Called when the game starts or when spawned
void ARTreasureChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARTreasureChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

