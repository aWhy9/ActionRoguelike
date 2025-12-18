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

bool URLAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;	
	Health = FMath::Clamp(Health, 0, MaxHealth);

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
}

float URLAttributeComponent::GetHealth() const
{
	return Health;
}

float URLAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

