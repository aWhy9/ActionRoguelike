// Fill out your copyright notice in the Description page of Project Settings.


#include "RLHealthPotion.h"
#include "RLAttributeComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "RLPlayerState.h"
#include "Kismet/GameplayStatics.h"


ARLHealthPotion::ARLHealthPotion()
{
	
	HealAmount = 25.0f;

}

void ARLHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{

	if (!ensure(InstigatorPawn))
	{
		return;
	}

	

	
	if (URLAttributeComponent* AttributeComponent = InstigatorPawn->FindComponentByClass<URLAttributeComponent>())
	{
		// Check if already at max health
		if (AttributeComponent->GetHealth() < AttributeComponent->GetMaxHealth())
		{
			bCanInteract = true;				
		}
		else
		{
			bCanInteract = false;return;
		}
		Super::Interact_Implementation(InstigatorPawn);
		AttributeComponent->ApplyHealthChange(this, HealAmount);			
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), InteractVFX, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), InteractSound, GetActorLocation());
		if (ARLPlayerState* PS = InstigatorPawn->GetPlayerState<ARLPlayerState>())
		{
			if (PS->Credits >= CreditCost)
			{
				PS->RemoveCredits(CreditCost);
			}			
		}
	}
		
}

