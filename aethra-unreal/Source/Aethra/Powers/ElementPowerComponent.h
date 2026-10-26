#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/AethraTypes.h"
#include "ElementPowerComponent.generated.h"

class AFireballProjectile;
class AAethraPlayerState;
class AAethraVehicle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAethraElementCast, EAethraElement, Element, FVector, Location);

// Energy, cooldowns and the five elements. Stats come from AethraRules so they match the prototype.
// Bind OnElementCast in Blueprint to play the Niagara effect and sound for each element.
// Networking TODO: route CastSelected through a Server RPC before multiplayer.
UCLASS(ClassGroup = (Aethra), meta = (BlueprintSpawnableComponent))
class AETHRA_API UElementPowerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UElementPowerComponent();

	UPROPERTY(BlueprintReadOnly, Category = "Powers") EAethraElement Selected = EAethraElement::Fire;
	UPROPERTY(BlueprintReadOnly, Category = "Powers") float Energy = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Powers") float EnergyPerSecond = 13.f;
	UPROPERTY(EditDefaultsOnly, Category = "Powers") TSubclassOf<AFireballProjectile> FireballClass;
	UPROPERTY(BlueprintAssignable, Category = "Powers") FAethraElementCast OnElementCast;

	UFUNCTION(BlueprintCallable, Category = "Powers") bool CastSelected();
	// In a car only Air (nitro) and Ice (freeze rivals) work.
	UFUNCTION(BlueprintCallable, Category = "Powers") bool CastInVehicle(AAethraVehicle* Vehicle);
	UFUNCTION(BlueprintCallable, Category = "Powers") void SelectNext(int32 Direction);
	UFUNCTION(BlueprintPure, Category = "Powers") float GetCooldownRemaining(EAethraElement Element) const;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Cooldowns[AethraRules::ElementCount] = {};

	float CachedMaxEnergy = 100.f;

	AAethraPlayerState* GetAethraPlayerState() const;
	bool CanCast(const AAethraPlayerState* PS, EAethraElement Element, int32& OutRank) const;
	void Spend(EAethraElement Element, int32 Rank);
	TArray<AActor*> PawnsInRadius(const FVector& Center, float RadiusCm) const;
};
