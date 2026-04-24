// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeCharacter.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "RActionComponent.h"
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

	ActionComponent = CreateDefaultSubobject<URActionComponent>("ActionComponent");
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";
}

void ARoguelikeCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ARoguelikeCharacter::OnHealthChanged);
}

FVector ARoguelikeCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
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

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARoguelikeCharacter::SpringtStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARoguelikeCharacter::SprintStop);
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


void ARoguelikeCharacter::SpringtStart()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void ARoguelikeCharacter::SprintStop()
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void ARoguelikeCharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}


void ARoguelikeCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}

void ARoguelikeCharacter::BlackHoleAttack()
{
	ActionComponent->StartActionByName(this, "Blackhole");	
}


void ARoguelikeCharacter::TeleportAbility()
{
	ActionComponent->StartActionByName(this, "Teleport");
}

void ARoguelikeCharacter::OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComponent,
	float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		
	}

	// On Death
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		SetLifeSpan(5.0f);
	}
}


void ARoguelikeCharacter::HealSelf(float Amount)
{
	AttributeComponent->ApplyHealthChange(this, Amount /* = 100 */);
}

