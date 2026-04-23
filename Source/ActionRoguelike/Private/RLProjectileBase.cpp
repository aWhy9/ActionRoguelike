// Fill out your copyright notice in the Description page of Project Settings.


#include "RLProjectileBase.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "UObject/FastReferenceCollector.h"

// Sets default values
ARLProjectileBase::ARLProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("Projectile");
	SphereComponent->OnComponentHit.AddDynamic(this, &ARLProjectileBase::OnActorHit);
	RootComponent = SphereComponent;
		
	EffectComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	EffectComponent->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 8000.f;
	MovementComponent->ProjectileGravityScale = 0.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->Radius = 500.0f;
	RadialForceComponent->ImpulseStrength = 2000.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->bAutoActivate = true;
	RadialForceComponent->bIgnoreOwningActor = true;	
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);


}	
	

void ARLProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ARLProjectileBase::Explode_Implementation()
{
	if (ensure(!IsPendingKillPending()))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		Destroy();
	}
}


void ARLProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}


