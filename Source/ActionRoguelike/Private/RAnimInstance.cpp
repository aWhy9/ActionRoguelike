// Fill out your copyright notice in the Description page of Project Settings.


#include "RAnimInstance.h"
#include "GameplayTagContainer.h"
#include "RActionComponent.h"

void URAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComp = Cast<URActionComponent>(OwningActor->GetComponentByClass(URActionComponent::StaticClass()));
	}
}

void URAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
