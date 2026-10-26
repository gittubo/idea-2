#include "Race/RaceCheckpoint.h"
#include "Race/RaceManager.h"
#include "Components/BoxComponent.h"

ARaceCheckpoint::ARaceCheckpoint()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetBoxExtent(FVector(300.f, 1300.f, 600.f)); // spans a 16 m road
	Trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = Trigger;
}

void ARaceCheckpoint::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ARaceCheckpoint::OnOverlap);
}

void ARaceCheckpoint::OnOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (Manager)
	{
		Manager->OnCheckpointReached(this, OtherActor);
	}
}
