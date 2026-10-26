// Game rules shared by every system: levels, element stats, opponent bonus and race payouts.
// Plain C++ with no engine types so it can be unit-tested outside Unreal (see Tests/RulesTest.cpp).
// Distances are in metres; Unreal code multiplies by 100 for centimetres.
#pragma once

#include <algorithm>
#include <array>
#include <cmath>

namespace AethraRules
{
	enum class EElement : unsigned char { Fire, Air, Ice, Earth, Lightning };
	inline constexpr int ElementCount = 5;
	inline constexpr int MaxRank = 5;
	inline constexpr int MaxLevel = 30;

	struct FElementInfo
	{
		const char* Name;
		int UnlockLevel;
		float EnergyCost;
		float CooldownSeconds;
		float BaseDamage;
		float BaseRadiusMetres; // area of effect, or search range for Lightning
	};

	inline constexpr std::array<FElementInfo, ElementCount> Elements = {{
		{ "Fire", 1, 14.f, 0.4f, 26.f, 0.f },
		{ "Air", 2, 20.f, 1.2f, 0.f, 5.f },
		{ "Ice", 3, 28.f, 3.0f, 12.f, 10.f },
		{ "Earth", 4, 34.f, 4.0f, 36.f, 12.f },
		{ "Lightning", 5, 30.f, 2.4f, 32.f, 34.f },
	}};

	inline const FElementInfo& Info(EElement E) { return Elements[static_cast<int>(E)]; }
	inline bool IsUnlocked(EElement E, int PlayerLevel) { return PlayerLevel >= Info(E).UnlockLevel; }

	inline int XpToNextLevel(int Level) { return 60 + Level * 60; }
	inline int MaxHealth(int Level) { return 100 + (Level - 1) * 12; }
	inline int MaxEnergy(int Level) { return 100 + (Level - 1) * 8; }

	inline float DamageMultiplier(int Rank) { return 1.f + 0.28f * (std::clamp(Rank, 1, MaxRank) - 1); }
	inline float Damage(EElement E, int Rank) { return Info(E).BaseDamage * DamageMultiplier(Rank); }
	inline float EnergyCost(EElement E, int Rank) { return Info(E).EnergyCost * (1.f - 0.06f * (std::clamp(Rank, 1, MaxRank) - 1)); }
	inline float Cooldown(EElement E, int Rank) { return Info(E).CooldownSeconds * (1.f - 0.08f * (std::clamp(Rank, 1, MaxRank) - 1)); }
	inline float RadiusMetres(EElement E, int Rank)
	{
		const float Grow = (E == EElement::Ice || E == EElement::Earth) ? 1.6f : 0.f;
		return Info(E).BaseRadiusMetres + Grow * (std::clamp(Rank, 1, MaxRank) - 1);
	}

	// Beating someone above your level pays more (up to 3x); beating someone far below pays almost nothing.
	inline float OpponentXpMultiplier(int OpponentLevel, int MyLevel)
	{
		const int Diff = OpponentLevel - MyLevel;
		return Diff > 0 ? std::min(3.f, 1.f + 0.25f * Diff) : std::max(0.15f, 1.f + 0.2f * Diff);
	}
	inline int DefeatXp(int OpponentLevel, int MyLevel)
	{
		return static_cast<int>(std::lround((22 + OpponentLevel * 12) * OpponentXpMultiplier(OpponentLevel, MyLevel)));
	}
	inline int DefeatShards(int OpponentLevel) { return 6 + OpponentLevel * 4; }

	struct FLevelResult { int Level; int Xp; int PowerPointsGained; };
	inline FLevelResult AddXp(int Level, int Xp, int Gain)
	{
		Xp += Gain;
		int Points = 0;
		while (Level < MaxLevel && Xp >= XpToNextLevel(Level)) { Xp -= XpToNextLevel(Level); ++Level; ++Points; }
		return { Level, Xp, Points };
	}

	// Every racer puts in the same stake; the winner takes the whole pot on top of the place prize.
	inline constexpr std::array<int, 4> RacePrize = { 120, 60, 30, 10 };
	inline constexpr std::array<int, 4> RaceXp = { 90, 55, 35, 20 };
	inline int RacePayout(int Place, int Stake, int Racers = 4)
	{
		if (Place < 1 || Place > static_cast<int>(RacePrize.size())) return 0;
		return RacePrize[Place - 1] + (Place == 1 ? Stake * Racers : 0);
	}
	inline int RaceXpFor(int Place) { return (Place < 1 || Place > 4) ? 0 : RaceXp[Place - 1]; }
}
