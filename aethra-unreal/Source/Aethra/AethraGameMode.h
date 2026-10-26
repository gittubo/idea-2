#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AethraGameMode.generated.h"

// Set Default Pawn Class to your character Blueprint in a Blueprint subclass of this game mode.
UCLASS()
class AETHRA_API AAethraGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAethraGameMode();
};
