#include "Player/AethraPlayerState.h"
#include "Net/UnrealNetwork.h"

AAethraPlayerState::AAethraPlayerState()
{
	ElementRanks.Init(1, AethraRules::ElementCount);
}

void AAethraPlayerState::GainXp(int32 Amount)
{
	const AethraRules::FLevelResult Result = AethraRules::AddXp(Level, Xp, Amount);
	const bool bLevelled = Result.Level != Level;
	Level = Result.Level;
	Xp = Result.Xp;
	PowerPoints += Result.PowerPointsGained;
	if (bLevelled)
	{
		OnLevelUp.Broadcast(Level, Result.PowerPointsGained);
	}
}

void AAethraPlayerState::AwardDefeat(int32 OpponentLevel)
{
	AddShards(AethraRules::DefeatShards(OpponentLevel));
	GainXp(AethraRules::DefeatXp(OpponentLevel, Level));
}

void AAethraPlayerState::AddShards(int32 Amount)
{
	Shards = FMath::Max(0, Shards + Amount);
}

bool AAethraPlayerState::TrySpendShards(int32 Amount)
{
	if (Amount < 0 || Shards < Amount)
	{
		return false;
	}
	Shards -= Amount;
	return true;
}

bool AAethraPlayerState::RankUp(EAethraElement Element)
{
	const int32 Index = static_cast<int32>(Element);
	if (PowerPoints < 1 || !IsElementUnlocked(Element) || !ElementRanks.IsValidIndex(Index) || ElementRanks[Index] >= AethraRules::MaxRank)
	{
		return false;
	}
	--PowerPoints;
	++ElementRanks[Index];
	return true;
}

int32 AAethraPlayerState::GetRank(EAethraElement Element) const
{
	const int32 Index = static_cast<int32>(Element);
	return ElementRanks.IsValidIndex(Index) ? ElementRanks[Index] : 1;
}

bool AAethraPlayerState::IsElementUnlocked(EAethraElement Element) const
{
	return AethraRules::IsUnlocked(ToRules(Element), Level);
}

void AAethraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAethraPlayerState, Level);
	DOREPLIFETIME(AAethraPlayerState, Xp);
	DOREPLIFETIME(AAethraPlayerState, PowerPoints);
	DOREPLIFETIME(AAethraPlayerState, Shards);
	DOREPLIFETIME(AAethraPlayerState, ElementRanks);
}
