#include "AethraGameMode.h"
#include "Player/AethraPlayerState.h"

AAethraGameMode::AAethraGameMode()
{
	PlayerStateClass = AAethraPlayerState::StaticClass();
}
