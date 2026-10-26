#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyDrone.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UFloatingPawnMovement;

// Rogue drone: hovers, chases players in range and pays XP to whoever defeats it,
// with the stronger-opponent bonus. Firing is left to the Blueprint (OnAttack).
UCLASS()
class AETHRA_API AEnemyDrone : public APawn
{
	GENERATED_BODY()

public:
	AEnemyDrone();

	UPROPERTY(VisibleAnywhere, Category = "Drone") TObjectPtr<USphereComponent> Collision;
	UPROPERTY(VisibleAnywhere, Category = "Drone") TObjectPtr<UStaticMeshComponent> Body;
	UPROPERTY(VisibleAnywhere, Category = "Drone") TObjectPtr<UFloatingPawnMovement> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drone", meta = (ClampMin = 1, ClampMax = 30)) int32 Level = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Drone") float AggroRange = 4600.f;
	UPROPERTY(EditDefaultsOnly, Category = "Drone") float HoverHeight = 500.f;
	UPROPERTY(EditDefaultsOnly, Category = "Drone") float AttackInterval = 2.2f;
	UPROPERTY(BlueprintReadOnly, Category = "Drone") float Health = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Drone") float FrozenTime = 0.f;

	UFUNCTION(BlueprintImplementableEvent, Category = "Drone") void OnAttack(APawn* Target);
	UFUNCTION(BlueprintImplementableEvent, Category = "Drone") void OnDefeated();

	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:
	FVector Home;
	float AttackTimer = 0.f;
	APawn* FindTarget() const;
};
