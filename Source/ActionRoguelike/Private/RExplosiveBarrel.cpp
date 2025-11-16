// Fill out your copyright notice in the Description page of Project Settings.


#include "RExplosiveBarrel.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsObjectBlueprintLibrary.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ARExplosiveBarrel::ARExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RootComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(RootComponent);

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(RootComponent);
	
	RadialForceComponent->Radius = 500.0f;
	RadialForceComponent->ImpulseStrength = 3000.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->bAutoActivate =  false;
	RadialForceComponent->bIgnoreOwningActor = true;
}

// Called when the game starts or when spawned
void ARExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	//SphereComponent->OnComponentHit.AddDynamic(this, &ARExplosiveBarrel::Explode(ImpulseRadius));
	
}

void ARExplosiveBarrel::Explode()
{
	RadialForceComponent->FireImpulse();

}

// Called every frame
void ARExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

