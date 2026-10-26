#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Core/AethraTypes.h"
#include "AethraPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAethraLevelUp, int32, NewLevel, int32, PowerPointsGained);

// Progression lives on the player state so it replicates to every client.
// Mutating functions are meant to run on the server.
UCLASS()
class AETHRA_API AAethraPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AAethraPlayerState();

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Progress") int32 Level = 1;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Progress") int32 Xp = 0;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Progress") int32 PowerPoints = 0;
	// Earned in play only; never bought and never cashed out (see the design document's Economy section).
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Progress") int32 Shards = 150;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Progress") TArray<int32> ElementRanks;

	UPROPERTY(BlueprintAssignable, Category = "Progress") FAethraLevelUp OnLevelUp;

	UFUNCTION(BlueprintCallable, Category = "Progress") void GainXp(int32 Amount);
	// XP and Shards for defeating an opponent, with the stronger-opponent bonus.
	UFUNCTION(BlueprintCallable, Category = "Progress") void AwardDefeat(int32 OpponentLevel);
	UFUNCTION(BlueprintCallable, Category = "Progress") void AddShards(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Progress") bool TrySpendShards(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Progress") bool RankUp(EAethraElement Element);

	UFUNCTION(BlueprintPure, Category = "Progress") int32 GetRank(EAethraElement Element) const;
	UFUNCTION(BlueprintPure, Category = "Progress") bool IsElementUnlocked(EAethraElement Element) const;
	UFUNCTION(BlueprintPure, Category = "Progress") int32 GetMaxHealth() const { return AethraRules::MaxHealth(Level); }
	UFUNCTION(BlueprintPure, Category = "Progress") int32 GetMaxEnergy() const { return AethraRules::MaxEnergy(Level); }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
