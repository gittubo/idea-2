#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaceCheckpoint.generated.h"

class UBoxComponent;
class ARaceManager;

// A gate on the race route. Place them along roads and add them, in order, to a RaceManager.
UCLASS()
class AETHRA_API ARaceCheckpoint : public AActor
{
	GENERATED_BODY()

public:
	ARaceCheckpoint();

	UPROPERTY(VisibleAnywhere, Category = "Race") TObjectPtr<UBoxComponent> Trigger;
	UPROPERTY(EditInstanceOnly, Category = "Race") TObjectPtr<ARaceManager> Manager;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
