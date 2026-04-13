// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "RLMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;
class USoundBase;
class UCameraShakeBase;
class URActionEffect;

UCLASS()
class ACTIONROGUELIKE_API ARLMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARLMagicProjectile();


protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<URActionEffect> BurningActionClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundBase* ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* ImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> CameraShake;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamageAmount;
	
protected:

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void ProjectileImapct();

	UFUNCTION (BlueprintCallable, Category="Camera",Meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static void PlayWorldCameraShake(const UObject * WorldContextObject, TSubclassOf <class UCameraShakeBase> Shake,	FVector Epicenter, float InnerRadius, float OuterRadius, float Falloff, bool bOrientShakeTowardsEpicenter);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
