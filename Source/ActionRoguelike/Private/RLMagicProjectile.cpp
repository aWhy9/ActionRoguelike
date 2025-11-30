// Fill out your copyright notice in the Description page of Project Settings.


#include "RLMagicProjectile.h"

#include "NiagaraComponent.h"
#include "RLAttributeComponent.h"
#include "RoguelikeCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ARLMagicProjectile::ARLMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("Projectile");
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARLMagicProjectile::OnActorOverlap);
	RootComponent = SphereComponent;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("EffectComponent");
	NiagaraComponent->SetupAttachment(SphereComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 1000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;
	
}


void ARLMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		URLAttributeComponent* AttributeComponent = Cast<URLAttributeComponent>(OtherActor->GetComponentByClass(URLAttributeComponent::StaticClass()));
		if (AttributeComponent)
			{
				AttributeComponent->ApplyHealthChange(-20.0f);

				Destroy();
			}
	}
}

// Called when the game starts or when spawned
void ARLMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARLMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

