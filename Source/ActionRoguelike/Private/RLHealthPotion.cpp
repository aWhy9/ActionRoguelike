// Fill out your copyright notice in the Description page of Project Settings.


#include "RLHealthPotion.h"
#include "RLAttributeComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


ARLHealthPotion::ARLHealthPotion()
{
	
	HealAmount = 25.0f;

}

void ARLHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	
	if (InstigatorPawn)
	{
		if (URLAttributeComponent* AttributeComponent = InstigatorPawn->FindComponentByClass<URLAttributeComponent>())
		{
			if (AttributeComponent->GetHealth() < AttributeComponent->GetMaxHealth())
			{
				bCanInteract = true;				
			}
			else
			{
				bCanInteract = false;
				return;
			}
			Super::Interact_Implementation(InstigatorPawn);
			AttributeComponent->ApplyHealthChange(HealAmount);			
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), InteractVFX, GetActorLocation());
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), InteractSound, GetActorLocation());	
		}
	}	
}

