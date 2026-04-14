// Fill out your copyright notice in the Description page of Project Settings.


#include "RTreasureChest.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ARTreasureChest::ARTreasureChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("Lid Mesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;
	bIsOpen = false;
	
	SetReplicates(true);
}

void ARTreasureChest::Interact_Implementation(APawn* InstigatorPawn)
{
	/*if (!bIsOpen)
	{
		LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
		bIsOpen = true;
	}
	else
	{
		LidMesh->SetRelativeRotation(FRotator(0, 0, 0));
		bIsOpen = false;
	}	*/

	bIsOpen = !bIsOpen;
	OnRep_LidOpened();
	
}

void ARTreasureChest::OnRep_LidOpened()
{
	float CurrPitch = bIsOpen ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

void ARTreasureChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTreasureChest, bIsOpen);
}
