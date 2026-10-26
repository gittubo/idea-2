#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "Core/AethraRules.h"
#include "AethraTypes.generated.h"

UENUM(BlueprintType)
enum class EAethraElement : uint8
{
	Fire,
	Air,
	Ice,
	Earth,
	Lightning
};

inline AethraRules::EElement ToRules(EAethraElement E) { return static_cast<AethraRules::EElement>(E); }

// Damage types let targets react to the element that hit them (Ice freezes, Earth knocks back).
UCLASS()
class AETHRA_API UIceDamageType : public UDamageType
{
	GENERATED_BODY()
};

UCLASS()
class AETHRA_API UEarthDamageType : public UDamageType
{
	GENERATED_BODY()
};
