#include "Race/RaceManager.h"
#include "Race/RaceCheckpoint.h"
#include "Vehicles/AethraVehicle.h"
#include "Player/AethraPlayerState.h"
#include "ChaosVehicleMovementComponent.h"
#include "TimerManager.h"

ARaceManager::ARaceManager()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

bool ARaceManager::StartRace(AAethraVehicle* PlayerVehicle, int32 InStake, bool bPowerRace)
{
	AAethraPlayerState* PS = PlayerVehicle ? PlayerVehicle->GetPlayerState<AAethraPlayerState>() : nullptr;
	if (bRacing || !PS || Checkpoints.Num() == 0 || GridSlots.Num() < 1 || !PS->TrySpendShards(InStake))
	{
		return false;
	}
	Stake = InStake;
	Racers.Reset();
	RaceTime = 0.f;

	auto PutOnGrid = [this](AAethraVehicle* V, int32 Slot)
	{
		const FTransform T = GridSlots[Slot] * GetActorTransform();
		V->SetActorTransform(T, false, nullptr, ETeleportType::ResetPhysics);
		V->bPowersAllowed = false; // until the countdown ends
	};

	auto AddRacer = [this](AAethraVehicle* V, const FString& Name, bool bPlayer)
	{
		FRacerState R;
		R.Vehicle = V;
		R.Name = Name;
		R.bIsPlayer = bPlayer;
		Racers.Add(R);
	};

	PutOnGrid(PlayerVehicle, 0);
	AddRacer(PlayerVehicle, PS->GetPlayerName(), true);
	for (int32 i = 1; i < GridSlots.Num() && RivalClass; ++i)
	{
		const FTransform T = GridSlots[i] * GetActorTransform();
		if (AAethraVehicle* Rival = GetWorld()->SpawnActor<AAethraVehicle>(RivalClass, T))
		{
			Rival->AutoDriveSpeed = 4000.f * FMath::FRandRange(0.84f, 0.95f);
			PutOnGrid(Rival, i);
			AddRacer(Rival, RivalNames.IsValidIndex(i - 1) ? RivalNames[i - 1] : FString::Printf(TEXT("Rival %d"), i), false);
		}
	}
	for (FRacerState& R : Racers)
	{
		R.Vehicle->Tags.AddUnique(bPowerRace ? TEXT("PowerRace") : TEXT("CleanRace"));
	}

	CountdownLeft = 3;
	OnCountdown.Broadcast(CountdownLeft);
	GetWorldTimerManager().SetTimer(CountdownTimer, this, &ARaceManager::CountdownStep, 1.f, true);
	return true;
}

void ARaceManager::CountdownStep()
{
	--CountdownLeft;
	OnCountdown.Broadcast(CountdownLeft);
	if (CountdownLeft > 0)
	{
		return;
	}
	GetWorldTimerManager().ClearTimer(CountdownTimer);
	bRacing = true;
	for (FRacerState& R : Racers)
	{
		R.Vehicle->bPowersAllowed = R.Vehicle->ActorHasTag(TEXT("PowerRace"));
		R.Vehicle->bAutoDrive = !R.bIsPlayer;
	}
}

float ARaceManager::Progress(const FRacerState& R) const
{
	if (R.Place > 0)
	{
		return 1e9f - R.Place;
	}
	const int32 Next = FMath::Min(R.NextCheckpoint, Checkpoints.Num() - 1);
	return R.NextCheckpoint * 1e6f - FVector::Dist(R.Vehicle->GetActorLocation(), Checkpoints[Next]->GetActorLocation());
}

int32 ARaceManager::GetPlayerPosition() const
{
	const FRacerState* Me = Racers.FindByPredicate([](const FRacerState& R) { return R.bIsPlayer; });
	if (!Me)
	{
		return 0;
	}
	int32 Position = 1;
	for (const FRacerState& R : Racers)
	{
		if (&R != Me && Progress(R) > Progress(*Me))
		{
			++Position;
		}
	}
	return Position;
}

void ARaceManager::OnCheckpointReached(ARaceCheckpoint* Checkpoint, AActor* Actor)
{
	if (!bRacing)
	{
		return;
	}
	FRacerState* R = Racers.FindByPredicate([Actor](const FRacerState& S) { return S.Vehicle.Get() == Actor; });
	if (!R || R->Place > 0 || !Checkpoints.IsValidIndex(R->NextCheckpoint) || Checkpoints[R->NextCheckpoint] != Checkpoint)
	{
		return; // wrong gate, or skipped one
	}
	++R->NextCheckpoint;
	if (R->NextCheckpoint >= Checkpoints.Num())
	{
		int32 Finished = 0;
		for (const FRacerState& S : Racers) { Finished += S.Place > 0 ? 1 : 0; }
		R->Place = Finished + 1;
		R->FinishTime = RaceTime;
		R->Vehicle->bAutoDrive = false;
		if (R->bIsPlayer)
		{
			FinishRace();
		}
	}
}

void ARaceManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!bRacing)
	{
		return;
	}
	RaceTime += DeltaSeconds;
	for (FRacerState& R : Racers)
	{
		if (!R.bIsPlayer && R.Place == 0 && Checkpoints.IsValidIndex(R.NextCheckpoint))
		{
			R.Vehicle->AutoDriveTarget = Checkpoints[R.NextCheckpoint]->GetActorLocation();
		}
	}
	if (RaceTime > TimeLimitSeconds)
	{
		FinishRace();
	}
}

void ARaceManager::FinishRace()
{
	bRacing = false;
	FRacerState* Me = Racers.FindByPredicate([](const FRacerState& R) { return R.bIsPlayer; });
	const int32 Place = Me && Me->Place > 0 ? Me->Place : GetPlayerPosition();
	const int32 Shards = AethraRules::RacePayout(Place, Stake, Racers.Num());
	const int32 Xp = AethraRules::RaceXpFor(Place);
	if (Me)
	{
		if (AAethraPlayerState* PS = Me->Vehicle->GetPlayerState<AAethraPlayerState>())
		{
			PS->AddShards(Shards);
			PS->GainXp(Xp);
		}
		Me->Vehicle->bPowersAllowed = true;
	}
	for (FRacerState& R : Racers)
	{
		R.Vehicle->Tags.Remove(TEXT("PowerRace"));
		R.Vehicle->Tags.Remove(TEXT("CleanRace"));
		if (!R.bIsPlayer)
		{
			R.Vehicle->SetLifeSpan(5.f);
		}
	}
	OnRaceFinished.Broadcast(Place, Shards, Xp);
}
