// Fill out your copyright notice in the Description page of Project Settings.


#include "RLTargetDummy.h"

#include "RLAttributeComponent.h"

// Sets default values
ARLTargetDummy::ARLTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMeshComponent;

	AttributeComponent = CreateDefaultSubobject<URLAttributeComponent>("AttributeComponent");

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ARLTargetDummy::OnHealthChanged);

}

void ARLTargetDummy::OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComponent, float NewHealth,	float Delta)
{
	if (Delta < 0.0f)
	{
		StaticMeshComponent->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
	
}


