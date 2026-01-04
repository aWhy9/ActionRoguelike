// Fill out your copyright notice in the Description page of Project Settings.


#include "RTreasureChest.h"


// Sets default values
ARTreasureChest::ARTreasureChest()
{
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
