#include "Vehicles/AethraVehicle.h"
#include "Player/AethraCharacter.h"
#include "Powers/ElementPowerComponent.h"
#include "Camera/CameraComponent.h"
#include "ChaosVehicleMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"

AAethraVehicle::AAethraVehicle()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 650.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 120.f);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraRotationLag = true;

	ChaseCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	ChaseCamera->SetupAttachment(CameraBoom);
}

void AAethraVehicle::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	if (const APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(VehicleMappingContext, 1);
		}
	}
}

void AAethraVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!Input)
	{
		return;
	}
	Input->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AAethraVehicle::Throttle);
	Input->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AAethraVehicle::Throttle);
	Input->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &AAethraVehicle::Brake);
	Input->BindAction(BrakeAction, ETriggerEvent::Completed, this, &AAethraVehicle::Brake);
	Input->BindAction(SteerAction, ETriggerEvent::Triggered, this, &AAethraVehicle::Steer);
	Input->BindAction(SteerAction, ETriggerEvent::Completed, this, &AAethraVehicle::Steer);
	Input->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &AAethraVehicle::HandbrakeOn);
	Input->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &AAethraVehicle::HandbrakeOff);
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAethraVehicle::Look);
	Input->BindAction(ExitAction, ETriggerEvent::Started, this, &AAethraVehicle::ExitVehicle);
	Input->BindAction(PowerAction, ETriggerEvent::Started, this, &AAethraVehicle::UsePower);
}

void AAethraVehicle::Throttle(const FInputActionValue& Value) { if (FreezeTime <= 0.f) GetVehicleMovementComponent()->SetThrottleInput(Value.Get<float>()); }
void AAethraVehicle::Brake(const FInputActionValue& Value) { GetVehicleMovementComponent()->SetBrakeInput(Value.Get<float>()); }
void AAethraVehicle::Steer(const FInputActionValue& Value) { GetVehicleMovementComponent()->SetSteeringInput(Value.Get<float>()); }
void AAethraVehicle::HandbrakeOn() { GetVehicleMovementComponent()->SetHandbrakeInput(true); }
void AAethraVehicle::HandbrakeOff() { GetVehicleMovementComponent()->SetHandbrakeInput(false); }

void AAethraVehicle::Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void AAethraVehicle::UsePower()
{
	if (Driver.IsValid())
	{
		Driver->Powers->CastInVehicle(this);
	}
}

void AAethraVehicle::EnterVehicle(AAethraCharacter* NewDriver)
{
	AController* PC = NewDriver ? NewDriver->GetController() : nullptr;
	if (!PC || HasDriver())
	{
		return;
	}
	Driver = NewDriver;
	bAutoDrive = false; // taking a car out of traffic
	NewDriver->SetActorHiddenInGame(true);
	NewDriver->SetActorEnableCollision(false);
	NewDriver->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PC->Possess(this);
}

void AAethraVehicle::ExitVehicle()
{
	AAethraCharacter* Out = Driver.Get();
	AController* PC = GetController();
	if (!Out || !PC || GetVehicleMovementComponent()->GetForwardSpeed() > 1200.f)
	{
		return;
	}
	GetVehicleMovementComponent()->SetThrottleInput(0.f);
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
	Out->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Out->SetActorLocation(GetActorLocation() - GetActorRightVector() * 220.f + FVector(0, 0, 100.f), false, nullptr, ETeleportType::TeleportPhysics);
	Out->SetActorHiddenInGame(false);
	Out->SetActorEnableCollision(true);
	Driver.Reset();
	PC->Possess(Out);
}

void AAethraVehicle::Boost(float Seconds) { BoostTime = FMath::Max(BoostTime, Seconds); }

void AAethraVehicle::Freeze(float Seconds)
{
	FreezeTime = FMath::Max(FreezeTime, Seconds);
	GetVehicleMovementComponent()->SetThrottleInput(0.f);
}

void AAethraVehicle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UChaosVehicleMovementComponent* Move = GetVehicleMovementComponent();

	if (FreezeTime > 0.f)
	{
		FreezeTime -= DeltaSeconds;
		Move->SetThrottleInput(0.f);
		Move->SetBrakeInput(1.f);
		if (FreezeTime <= 0.f)
		{
			Move->SetBrakeInput(0.f);
		}
		return;
	}
	if (BoostTime > 0.f)
	{
		BoostTime -= DeltaSeconds;
		GetMesh()->AddForce(GetActorForwardVector() * BoostAcceleration, NAME_None, true);
	}
	if (bAutoDrive)
	{
		// steer toward the target and ease off for sharp turns, like the prototype's rivals
		const FVector ToTarget = (AutoDriveTarget - GetActorLocation()).GetSafeNormal2D();
		const float Side = FVector::DotProduct(GetActorRightVector(), ToTarget);
		const float Ahead = FVector::DotProduct(GetActorForwardVector(), ToTarget);
		const float Angle = FMath::Atan2(Side, Ahead);
		const float WantSpeed = FMath::Abs(Angle) > 0.35f ? AutoDriveSpeed * 0.55f : AutoDriveSpeed;
		const float Speed = Move->GetForwardSpeed();
		Move->SetSteeringInput(FMath::Clamp(Angle * 2.2f, -1.f, 1.f));
		Move->SetThrottleInput(Speed < WantSpeed ? 1.f : 0.f);
		Move->SetBrakeInput(Speed > WantSpeed + 300.f ? 0.4f : 0.f);
	}
}
