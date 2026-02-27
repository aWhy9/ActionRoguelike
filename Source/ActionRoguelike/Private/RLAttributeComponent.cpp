// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAttributeComponent.h"

#include "RGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("rl.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
URLAttributeComponent::URLAttributeComponent()
{
	MaxHealth = 100;
	Health = 100;
}

bool URLAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

bool URLAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

float URLAttributeComponent::GetHealth() const
{
	return Health;
}

float URLAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

bool URLAttributeComponent::IsFullHealth() const
{
	return Health == MaxHealth;
}

bool URLAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	UE_LOG(LogTemp, Warning, TEXT("Instigator: %p  Delta: %f"), InstigatorActor, Delta);
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}
	
	float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	// Check to see if health actually changed, I.E. already at 0
	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	// On Death
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ARGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARGameModeBase>();
		if (GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}		
	}
	
	return ActualDelta != 0;
}

URLAttributeComponent* URLAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<URLAttributeComponent>();
	}
	return nullptr;
}

bool URLAttributeComponent::IsActorAlive(AActor* Actor)
{
	URLAttributeComponent* AttributeComponent = GetAttributes(Actor);
	if (AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}

	return false;
}

