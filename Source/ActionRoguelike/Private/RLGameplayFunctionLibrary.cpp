// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameplayFunctionLibrary.h"
#include "RLAttributeComponent.h"

bool URLGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	URLAttributeComponent* AttributeComponent = URLAttributeComponent::GetAttributes(TargetActor);
	UE_LOG(LogTemp, Warning, TEXT("APPLY DAMAGE TGT ACTOR: %p & ATTR COMP: %p"), TargetActor, AttributeComponent);
	if (AttributeComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Before Call - Instigator: %p  Delta: %f"), DamageCauser, DamageAmount);
		return AttributeComponent->ApplyHealthChange(DamageCauser, DamageAmount);
	}
	return false;
}

bool URLGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{	
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{	
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			// Direction = Target - Origin
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();
				
			HitComp->AddImpulseAtLocation(Direction * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}	
		return true;
	}
	//UE_LOG(LogTemp, Warning, TEXT("IMPULSE fAILED"));
	return false;
}

