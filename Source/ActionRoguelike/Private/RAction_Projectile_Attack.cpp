// Fill out your copyright notice in the Description page of Project Settings.


#include "RAction_Projectile_Attack.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"

// Constructor
URAction_Projectile_Attack::URAction_Projectile_Attack()
{

	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}

void URAction_Projectile_Attack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);
		
		// Play a particle effect on handsocket
		UNiagaraFunctionLibrary::SpawnSystemAttached(CastingVFX, Character->GetMesh(),HandSocketName,  FVector::ZeroVector,  FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);

		 
	}	
}

void URAction_Projectile_Attack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		// Handle aiming of projectile
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		/////////////////// DEBUG FOR TESTING
		/*FString Msg = FString::Printf(TEXT("VFX: %p"), CastingVFX);
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,             // Key (-1 = new message every time)
				5.0f,           // Duration (seconds)
				FColor::Green,  // Text color
				Msg
			);
		}*/
		////////////////////////// DEBUG

		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore Player
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();

		// Endpoint far into the look-at distance (not too far, still adjust somewhat towwards crosshair on miss)
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000);

		FHitResult Hit;
		// returns true if we got a blocking hit
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape, CollisionParams))
		{

			// Overwrite trace end with impact point in world
			TraceEnd = Hit.ImpactPoint;
		}

		// fnd new direction/rotation from hand pointing to impact point in world
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}

