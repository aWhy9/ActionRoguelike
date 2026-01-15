// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeCharacter.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "RInteractionComponent.h"
#include "RLAttributeComponent.h"
#include "RLMagicProjectile.h"
#include "RLProjectileBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARoguelikeCharacter::ARoguelikeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	InteractionComponent = CreateDefaultSubobject<URInteractionComponent>("InteractionComponent");
	
	AttributeComponent = CreateDefaultSubobject<URLAttributeComponent>("AttributeComponent");
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;

	HandSocketName = "Muzzle_01";

	TimeToHitParamName = "TimeToHit";
}

void ARoguelikeCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ARoguelikeCharacter::OnHealthChanged);
}


// Called when the game starts or when spawned
void ARoguelikeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoguelikeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);

}

// Called to bind functionality to input
void ARoguelikeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARoguelikeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARoguelikeCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ARoguelikeCharacter::PrimaryAttack);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ARoguelikeCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ARoguelikeCharacter::BlackHoleAttack);

	PlayerInputComponent->BindAction("TeleportAbility", IE_Pressed, this, &ARoguelikeCharacter::TeleportAbility);
}

void ARoguelikeCharacter::MoveForward(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
	
	AddMovementInput(ControlRotation.Vector(), Value);
}

void ARoguelikeCharacter::MoveRight(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	// X =  Forward
	// Y = Right
	// Z = Up
	
	FVector RightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);	
}

void ARoguelikeCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		// Handle aiming of projectile
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

		// Play a particle effect on hand when attacking/casting
		UNiagaraFunctionLibrary::SpawnSystemAttached(CastingVFX, GetMesh(),HandSocketName,  FVector::ZeroVector,  FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);


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
		SpawnParams.Instigator = this;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore Player
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = CameraComponent->GetComponentLocation();

		// Endpoint far into the look-at distance (not too far, still adjust somewhat towwards crosshair on miss)
		FVector TraceEnd = CameraComponent->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

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
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
	
}

void ARoguelikeCharacter::PrimaryAttack()
{

	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ARoguelikeCharacter::PrimaryAttack_TimeElapsed, 0.2f);

	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);	
}
void ARoguelikeCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
	
	
}

void ARoguelikeCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}

void ARoguelikeCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAnim);
	
	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAttack, this, &ARoguelikeCharacter::BlackHoleAttack_TimeElapsed, 0.2f);	
	
}

void ARoguelikeCharacter::BlackHoleAttack_TimeElapsed()
{
	if (BlackHoleClass)
	{
		FVector CameraLocation = CameraComponent->GetComponentLocation();
		FRotator CameraRotation = CameraComponent->GetComponentRotation();
		FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * 1000);
	
		//Handle aiming of projectile
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
		FRotator AimRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TraceEnd);
	
		FTransform SpawnTM = FTransform(AimRotation, HandLocation);
	
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;	
	
		GetWorld()->SpawnActor<AActor>(BlackHoleClass, SpawnTM, SpawnParams);
	}
	
}

void ARoguelikeCharacter::TeleportAbility()
{

	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_TeleportAbility, this, &ARoguelikeCharacter::TeleportAbility_TimeElapsed, 0.2f);	
}

void ARoguelikeCharacter::TeleportAbility_TimeElapsed()
{
	if (TeleportProjectileClass)
	{
		FVector CameraLocation = CameraComponent->GetComponentLocation();
		FRotator CameraRotation = CameraComponent->GetComponentRotation();
		FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * 1000);
	
		//Handle aiming of projectile
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
		FRotator AimRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TraceEnd);
	
		FTransform SpawnTM = FTransform(AimRotation, HandLocation);
	
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;	
	
		GetWorld()->SpawnActor<AActor>(TeleportProjectileClass, SpawnTM, SpawnParams);
	}
	
}

void ARoguelikeCharacter::OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComponent,
	float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		
	}
	
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}


void ARoguelikeCharacter::HealSelf(float Amount)
{
	AttributeComponent->ApplyHealthChange(this, Amount /* = 100 */);
}

