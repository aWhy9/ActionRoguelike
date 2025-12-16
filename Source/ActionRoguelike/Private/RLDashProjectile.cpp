// Fill out your copyright notice in the Description page of Project Settings.


#include "RLDashProjectile.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARLDashProjectile::ARLDashProjectile()
{	
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MovementComponent->InitialSpeed = 6000.0f;
}

void ARLDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimeHandle_DelayedDetonate, this, &ARLDashProjectile::Explode, DetonateDelay);

}

//_implementation needed for code but is the same as Explode, an unreal caveat
void ARLDashProjectile::Explode_Implementation()
{
	// Clear timer if the Explode was already called through another source like OnActorHit
	GetWorldTimerManager().ClearTimer(TimeHandle_DelayedDetonate);

	// Play effect and sound upon detonation
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation(), GetActorRotation());
	
	EffectComponent->DeactivateImmediate();

	MovementComponent->StopMovementImmediately();
	SetActorEnableCollision(false);
	
	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ARLDashProjectile::TeleportInstigator, TeleportDelay);

	
	// Skip base implementation as it will destroy actor (we need to stay alive a bit longer to finish the 2nd timer
	// Super::Explode_Implementation();

	FVector Loc = GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("Explode at Z = %f"), Loc.Z);
}

void ARLDashProjectile::TeleportInstigator()
{
	
	AActor* ActorToTeleport = GetInstigator();	

	// Add some Z space as otherwise it doesn't trigger the teleport
	FVector TargetLocation = GetActorLocation() + FVector(0.f, 0.f, 50.f); 	
	
	if (ensure(ActorToTeleport))
	{
		
		// Keep Instigator rotation or it may end up jarring
		ActorToTeleport->TeleportTo(TargetLocation, ActorToTeleport->GetActorRotation(), false, false);
	}
}

