#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AethraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UElementPowerComponent;
struct FInputActionValue;

// Third-person player. Make a Blueprint subclass, set its mesh to the player's MetaHuman
// (or a template mannequin while prototyping), and assign the input assets below.
UCLASS()
class AETHRA_API AAethraCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAethraCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Powers") TObjectPtr<UElementPowerComponent> Powers;

	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> CastAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> NextElementAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> PreviousElementAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Movement") float JogSpeed = 520.f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement") float SprintSpeed = 850.f;
	UPROPERTY(EditDefaultsOnly, Category = "Vehicles") float EnterVehicleRange = 450.f;

	UPROPERTY(BlueprintReadOnly, Category = "Health") float Health = 100.f;

	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSprint();
	void StopSprint();
	void CastPower();
	void NextElement();
	void PreviousElement();
	void Interact();

	// Called when health reaches zero; respawn rules live in Blueprint.
	UFUNCTION(BlueprintImplementableEvent, Category = "Health") void OnKnockedOut();
};
