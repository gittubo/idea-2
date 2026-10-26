#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaceManager.generated.h"

class ARaceCheckpoint;
class AAethraVehicle;

USTRUCT(BlueprintType)
struct FRacerState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) TObjectPtr<AAethraVehicle> Vehicle = nullptr;
	UPROPERTY(BlueprintReadOnly) FString Name;
	UPROPERTY(BlueprintReadOnly) bool bIsPlayer = false;
	UPROPERTY(BlueprintReadOnly) int32 NextCheckpoint = 0;
	UPROPERTY(BlueprintReadOnly) int32 Place = 0; // 0 until finished
	UPROPERTY(BlueprintReadOnly) float FinishTime = 0.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAethraRaceFinished, int32, Place, int32, ShardsWon, int32, XpWon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAethraCountdown, int32, SecondsLeft);

// Runs a street race: takes the stake, lines up the grid, drives the rivals, tracks checkpoints
// and pays out with the same rules as the prototype (AethraRules::RacePayout).
UCLASS()
class AETHRA_API ARaceManager : public AActor
{
	GENERATED_BODY()

public:
	ARaceManager();

	UPROPERTY(EditInstanceOnly, Category = "Race") TArray<TObjectPtr<ARaceCheckpoint>> Checkpoints;
	UPROPERTY(EditInstanceOnly, Category = "Race", meta = (MakeEditWidget)) TArray<FTransform> GridSlots;
	UPROPERTY(EditDefaultsOnly, Category = "Race") TSubclassOf<AAethraVehicle> RivalClass;
	UPROPERTY(EditDefaultsOnly, Category = "Race") TArray<FString> RivalNames = { TEXT("Vex"), TEXT("Kira-9"), TEXT("Duke Rami"), TEXT("Solene") };
	UPROPERTY(EditDefaultsOnly, Category = "Race") float TimeLimitSeconds = 150.f;

	UPROPERTY(BlueprintReadOnly, Category = "Race") TArray<FRacerState> Racers;
	UPROPERTY(BlueprintReadOnly, Category = "Race") bool bRacing = false;
	UPROPERTY(BlueprintReadOnly, Category = "Race") float RaceTime = 0.f;

	UPROPERTY(BlueprintAssignable, Category = "Race") FAethraRaceFinished OnRaceFinished;
	UPROPERTY(BlueprintAssignable, Category = "Race") FAethraCountdown OnCountdown;

	// Returns false if the player cannot cover the stake or a race is running.
	UFUNCTION(BlueprintCallable, Category = "Race") bool StartRace(AAethraVehicle* PlayerVehicle, int32 Stake, bool bPowerRace);
	UFUNCTION(BlueprintPure, Category = "Race") int32 GetPlayerPosition() const;

	void OnCheckpointReached(ARaceCheckpoint* Checkpoint, AActor* Actor);

	virtual void Tick(float DeltaSeconds) override;

private:
	int32 Stake = 0;
	int32 CountdownLeft = 0;
	FTimerHandle CountdownTimer;

	void CountdownStep();
	float Progress(const FRacerState& R) const;
	void FinishRace();
};
