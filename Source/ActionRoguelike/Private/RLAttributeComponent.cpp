// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAttributeComponent.h"

#include "RGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("rl.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
URLAttributeComponent::URLAttributeComponent()
{
	MaxHealth = 100;
	Health = 100;
	MaxRage = 100;
	Rage = 0;

	SetIsReplicatedByDefault(true);
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

		Rage += -Delta;
	}
	
	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	// Check to see if health actually changed, I.E. already at 0
	float ActualDelta = NewHealth - OldHealth;
	
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}
		
		// On Death
		if (ActualDelta < 0.0f && Health == 0.0f)
		{
			ARGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARGameModeBase>();
			if (GameMode)
			{
				GameMode->OnActorKilled(GetOwner(), InstigatorActor);
			}		
		}	
	}
	
	return ActualDelta != 0;
}

bool URLAttributeComponent::ApplyRageChange(float Delta)
{
	if (Rage > 0)
	{
		Rage -= Delta;
		return true;	
	}
	return false;	
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

void URLAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}


void URLAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URLAttributeComponent, Health);
	DOREPLIFETIME(URLAttributeComponent, MaxHealth);
	//DOREPLIFETIME_CONDITION(URLAttributeComponent, MaxHealth, COND_OwnerOnly); // FOR OPTIMIZATION
}