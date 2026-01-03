// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAttributeComponent.h"

// Sets default values for this component's properties
URLAttributeComponent::URLAttributeComponent()
{
	MaxHealth = 100;
	Health = 100;
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

bool URLAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	// Check to see if health actually changed, I.E. already at 0
	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);
	
	return ActualDelta != 0;
}

