// Fill out your copyright notice in the Description page of Project Settings.


#include "RLMagicProjectile.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "RLAttributeComponent.h"
#include "RLGameplayFunctionLibrary.h"
#include "RoguelikeCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARLMagicProjectile::ARLMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageAmount = -20.0f;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("Projectile");
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARLMagicProjectile::OnActorOverlap);
	RootComponent = SphereComponent;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("EffectComponent");
	NiagaraComponent->SetupAttachment(SphereComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 4000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ARLMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentHit.AddDynamic(this, &ARLMagicProjectile::OnComponentHit);
	
}

void ARLMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARLMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,	bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OVERLAP"));
	if (OtherActor && OtherActor != GetInstigator())
	{
		/*URLAttributeComponent* AttributeComponent = Cast<URLAttributeComponent>(OtherActor->GetComponentByClass(URLAttributeComponent::StaticClass()));
		if (AttributeComponent)
		{
			AttributeComponent->ApplyHealthChange(GetInstigator(), DamageAmount);
			ProjectileImapct();
		}*/
		if (URLGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			UE_LOG(LogTemp, Warning, TEXT("APPLYING DIR DAMAGE"));
			ProjectileImapct();
		}
	}
}


void ARLMagicProjectile::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ProjectileImapct();
}

void ARLMagicProjectile::ProjectileImapct()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactVFX,GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation(), GetActorRotation());

	if (CameraShake)
	{		
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, GetActorLocation(), 600, 2000, 400, true);
	}
	
	Destroy();		
}

void ARLMagicProjectile::PlayWorldCameraShake(const UObject* WorldContextObject,
	TSubclassOf<class UCameraShakeBase> Shake, FVector Epicenter, float InnerRadius, float OuterRadius, float Falloff,
	bool bOrientShakeTowardsEpicenter)
{
	
}


