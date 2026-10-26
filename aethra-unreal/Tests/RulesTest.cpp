// Standalone checks for AethraRules.h. Build and run:
//   g++ -std=c++20 -I../Source/Aethra/Core RulesTest.cpp -o rules_test && ./rules_test
#include "AethraRules.h"
#include <cmath>
#include <cstdio>

using namespace AethraRules;

static int Failures = 0;
#define CHECK(Cond) do { if (!(Cond)) { std::printf("FAIL line %d: %s\n", __LINE__, #Cond); ++Failures; } } while (0)
static bool Near(float A, float B) { return std::fabs(A - B) < 1e-4f; }

int main()
{
	// levels
	CHECK(XpToNextLevel(1) == 120);
	CHECK(XpToNextLevel(5) == 360);
	auto R = AddXp(1, 0, 119); CHECK(R.Level == 1 && R.Xp == 119 && R.PowerPointsGained == 0);
	R = AddXp(1, 0, 120); CHECK(R.Level == 2 && R.Xp == 0 && R.PowerPointsGained == 1);
	R = AddXp(1, 0, 120 + 180 + 5); CHECK(R.Level == 3 && R.Xp == 5 && R.PowerPointsGained == 2);
	R = AddXp(MaxLevel, 0, 100000); CHECK(R.Level == MaxLevel && R.PowerPointsGained == 0);
	CHECK(MaxHealth(1) == 100 && MaxHealth(6) == 160);

	// stronger-opponent bonus
	CHECK(Near(OpponentXpMultiplier(5, 5), 1.f));
	CHECK(Near(OpponentXpMultiplier(7, 5), 1.5f));
	CHECK(Near(OpponentXpMultiplier(20, 1), 3.f));      // capped
	CHECK(Near(OpponentXpMultiplier(1, 3), 0.6f));
	CHECK(Near(OpponentXpMultiplier(1, 30), 0.15f));    // floor
	CHECK(DefeatXp(7, 5) == 159);                        // (22 + 84) * 1.5
	CHECK(DefeatXp(3, 3) == 58);

	// elements
	CHECK(IsUnlocked(EElement::Fire, 1) && !IsUnlocked(EElement::Air, 1) && IsUnlocked(EElement::Lightning, 5));
	CHECK(Near(Damage(EElement::Fire, 1), 26.f));
	CHECK(Near(Damage(EElement::Fire, 5), 26.f * 2.12f));
	CHECK(Near(Damage(EElement::Fire, 99), Damage(EElement::Fire, 5)));   // rank clamped
	CHECK(Near(RadiusMetres(EElement::Ice, 3), 13.2f));
	CHECK(Near(RadiusMetres(EElement::Lightning, 5), 34.f));
	CHECK(Near(Cooldown(EElement::Earth, 5), 4.f * 0.68f));
	CHECK(Near(EnergyCost(EElement::Ice, 2), 28.f * 0.94f));

	// races
	CHECK(RacePayout(1, 0) == 120);
	CHECK(RacePayout(1, 50) == 320);
	CHECK(RacePayout(2, 50) == 60);
	CHECK(RacePayout(4, 250) == 10);
	CHECK(RacePayout(5, 50) == 0);
	CHECK(RaceXpFor(1) == 90 && RaceXpFor(4) == 20 && RaceXpFor(0) == 0);

	std::printf(Failures ? "%d check(s) failed\n" : "All rules checks passed\n", Failures);
	return Failures ? 1 : 0;
}
