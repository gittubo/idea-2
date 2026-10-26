#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "AethraVehicle.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AAethraCharacter;
struct FInputActionValue;

// Drivable Chaos vehicle. Base a Blueprint on this with a car mesh and wheel setup
// (Unreal's Vehicle template shows how). Also drives itself for race rivals and traffic.
UCLASS()
class AETHRA_API AAethraVehicle : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	AAethraVehicle();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") TObjectPtr<UCameraComponent> ChaseCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputMappingContext> VehicleMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> ThrottleAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> BrakeAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> SteerAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> HandbrakeAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> ExitAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> PowerAction;

	UPROPERTY(EditDefaultsOnly, Category = "Boost") float BoostAcceleration = 900.f; // cm/s^2
	// False during a Clean Race.
	UPROPERTY(BlueprintReadWrite, Category = "Powers") bool bPowersAllowed = true;

	// Self-driving for rivals and traffic: steer toward AutoDriveTarget at up to AutoDriveSpeed.
	UPROPERTY(BlueprintReadWrite, Category = "AI") bool bAutoDrive = false;
	UPROPERTY(BlueprintReadWrite, Category = "AI") FVector AutoDriveTarget = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, Category = "AI") float AutoDriveSpeed = 4000.f; // cm/s

	void EnterVehicle(AAethraCharacter* NewDriver);
	UFUNCTION(BlueprintCallable, Category = "Vehicle") void ExitVehicle();
	UFUNCTION(BlueprintCallable, Category = "Vehicle") void Boost(float Seconds);
	UFUNCTION(BlueprintCallable, Category = "Vehicle") void Freeze(float Seconds);
	UFUNCTION(BlueprintPure, Category = "Vehicle") bool HasDriver() const { return Driver.IsValid(); }

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Throttle(const FInputActionValue& Value);
	void Brake(const FInputActionValue& Value);
	void Steer(const FInputActionValue& Value);
	void HandbrakeOn();
	void HandbrakeOff();
	void Look(const FInputActionValue& Value);
	void UsePower();

private:
	TWeakObjectPtr<AAethraCharacter> Driver;
	float BoostTime = 0.f;
	float FreezeTime = 0.f;
};
