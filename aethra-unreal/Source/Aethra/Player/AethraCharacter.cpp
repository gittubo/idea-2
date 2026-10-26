#include "Player/AethraCharacter.h"
#include "Player/AethraPlayerState.h"
#include "Powers/ElementPowerComponent.h"
#include "Vehicles/AethraVehicle.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"

AAethraCharacter::AAethraCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 92.f);
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.35f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->SocketOffset = FVector(0.f, 60.f, 60.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	Powers = CreateDefaultSubobject<UElementPowerComponent>(TEXT("Powers"));
}

void AAethraCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	if (const APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AAethraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!Input)
	{
		return;
	}
	Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAethraCharacter::Move);
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAethraCharacter::Look);
	Input->BindAction(SprintAction, ETriggerEvent::Started, this, &AAethraCharacter::StartSprint);
	Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &AAethraCharacter::StopSprint);
	Input->BindAction(CastAction, ETriggerEvent::Started, this, &AAethraCharacter::CastPower);
	Input->BindAction(NextElementAction, ETriggerEvent::Started, this, &AAethraCharacter::NextElement);
	Input->BindAction(PreviousElementAction, ETriggerEvent::Started, this, &AAethraCharacter::PreviousElement);
	Input->BindAction(InteractAction, ETriggerEvent::Started, this, &AAethraCharacter::Interact);
}

void AAethraCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	if (!Controller)
	{
		return;
	}
	const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
	AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::X), Axis.Y);
	AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y), Axis.X);
}

void AAethraCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void AAethraCharacter::StartSprint() { GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; }
void AAethraCharacter::StopSprint() { GetCharacterMovement()->MaxWalkSpeed = JogSpeed; }
void AAethraCharacter::CastPower() { Powers->CastSelected(); }
void AAethraCharacter::NextElement() { Powers->SelectNext(1); }
void AAethraCharacter::PreviousElement() { Powers->SelectNext(-1); }

void AAethraCharacter::Interact()
{
	// get into the nearest car in range; any car in the city can be driven
	AAethraVehicle* Nearest = nullptr;
	float NearestDist = EnterVehicleRange;
	for (TActorIterator<AAethraVehicle> It(GetWorld()); It; ++It)
	{
		const float Dist = FVector::Dist(It->GetActorLocation(), GetActorLocation());
		if (Dist < NearestDist && !It->HasDriver())
		{
			NearestDist = Dist;
			Nearest = *It;
		}
	}
	if (Nearest)
	{
		Nearest->EnterVehicle(this);
	}
}

float AAethraCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (Health > 0.f)
	{
		Health = FMath::Max(0.f, Health - Applied);
		if (Health <= 0.f)
		{
			OnKnockedOut();
		}
	}
	return Applied;
}
