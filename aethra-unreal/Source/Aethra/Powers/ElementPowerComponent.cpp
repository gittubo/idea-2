#include "Powers/ElementPowerComponent.h"
#include "Powers/FireballProjectile.h"
#include "Player/AethraPlayerState.h"
#include "Vehicles/AethraVehicle.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UElementPowerComponent::UElementPowerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UElementPowerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// while driving, the vehicle holds the player state, so keep the last known cap
	if (const AAethraPlayerState* PS = GetAethraPlayerState())
	{
		CachedMaxEnergy = PS->GetMaxEnergy();
	}
	Energy = FMath::Min(CachedMaxEnergy, Energy + EnergyPerSecond * DeltaTime);
	for (float& C : Cooldowns)
	{
		C = FMath::Max(0.f, C - DeltaTime);
	}
}

AAethraPlayerState* UElementPowerComponent::GetAethraPlayerState() const
{
	const APawn* Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetPlayerState<AAethraPlayerState>() : nullptr;
}

float UElementPowerComponent::GetCooldownRemaining(EAethraElement Element) const
{
	return Cooldowns[static_cast<int32>(Element)];
}

void UElementPowerComponent::SelectNext(int32 Direction)
{
	const AAethraPlayerState* PS = GetAethraPlayerState();
	int32 Index = static_cast<int32>(Selected);
	for (int32 Step = 0; Step < AethraRules::ElementCount; ++Step)
	{
		Index = (Index + (Direction >= 0 ? 1 : AethraRules::ElementCount - 1)) % AethraRules::ElementCount;
		if (!PS || PS->IsElementUnlocked(static_cast<EAethraElement>(Index)))
		{
			Selected = static_cast<EAethraElement>(Index);
			return;
		}
	}
}

bool UElementPowerComponent::CanCast(const AAethraPlayerState* PS, EAethraElement Element, int32& OutRank) const
{
	if (!PS || !PS->IsElementUnlocked(Element) || Cooldowns[static_cast<int32>(Element)] > 0.f)
	{
		return false;
	}
	OutRank = PS->GetRank(Element);
	return Energy >= AethraRules::EnergyCost(ToRules(Element), OutRank);
}

void UElementPowerComponent::Spend(EAethraElement Element, int32 Rank)
{
	Energy -= AethraRules::EnergyCost(ToRules(Element), Rank);
	Cooldowns[static_cast<int32>(Element)] = AethraRules::Cooldown(ToRules(Element), Rank);
}

TArray<AActor*> UElementPowerComponent::PawnsInRadius(const FVector& Center, float RadiusCm) const
{
	TArray<AActor*> Found;
	const TArray<TEnumAsByte<EObjectTypeQuery>> Types = { UEngineTypes::ConvertToObjectType(ECC_Pawn) };
	const TArray<AActor*> Ignore = { GetOwner() };
	UKismetSystemLibrary::SphereOverlapActors(this, Center, RadiusCm, Types, nullptr, Ignore, Found);
	return Found;
}

bool UElementPowerComponent::CastSelected()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	int32 Rank = 1;
	if (!Owner || !CanCast(GetAethraPlayerState(), Selected, Rank))
	{
		return false;
	}
	const AethraRules::EElement E = ToRules(Selected);
	const float Damage = AethraRules::Damage(E, Rank);
	const float RadiusCm = AethraRules::RadiusMetres(E, Rank) * 100.f;
	const FVector Origin = Owner->GetActorLocation();
	AController* Instigator = Owner->GetController();

	switch (Selected)
	{
	case EAethraElement::Fire:
	{
		if (!FireballClass)
		{
			return false;
		}
		FVector EyeLocation;
		FRotator Aim;
		Owner->GetActorEyesViewPoint(EyeLocation, Aim);
		if (Instigator)
		{
			Aim = Instigator->GetControlRotation();
		}
		const FTransform SpawnAt(Aim, Origin + Aim.Vector() * 80.f + FVector(0, 0, 40.f));
		AFireballProjectile* Ball = GetWorld()->SpawnActorDeferred<AFireballProjectile>(FireballClass, SpawnAt, Owner, Owner);
		if (Ball)
		{
			Ball->Damage = Damage;
			UGameplayStatics::FinishSpawningActor(Ball, SpawnAt);
		}
		break;
	}
	case EAethraElement::Air:
		Owner->LaunchCharacter(Owner->GetActorForwardVector() * (1600.f + 200.f * Rank) + FVector(0, 0, 900.f + 100.f * Rank), true, true);
		break;
	case EAethraElement::Ice:
		UGameplayStatics::ApplyRadialDamage(this, Damage, Origin, RadiusCm, UIceDamageType::StaticClass(), { Owner }, Owner, Instigator, true);
		break;
	case EAethraElement::Earth:
		UGameplayStatics::ApplyRadialDamage(this, Damage, Origin, RadiusCm, UEarthDamageType::StaticClass(), { Owner }, Owner, Instigator, true);
		for (AActor* Hit : PawnsInRadius(Origin, RadiusCm))
		{
			if (ACharacter* Victim = Cast<ACharacter>(Hit))
			{
				const FVector Away = (Victim->GetActorLocation() - Origin).GetSafeNormal2D();
				Victim->LaunchCharacter(Away * 2200.f + FVector(0, 0, 1000.f), true, true);
			}
		}
		break;
	case EAethraElement::Lightning:
	{
		// strike the nearest target, then jump to up to two more nearby
		TArray<AActor*> Struck;
		FVector From = Origin;
		for (int32 Jump = 0; Jump < 3; ++Jump)
		{
			const float Range = Jump == 0 ? RadiusCm : 1500.f;
			AActor* Best = nullptr;
			float BestDist = Range;
			for (AActor* Candidate : PawnsInRadius(From, Range))
			{
				const float Dist = FVector::Dist(From, Candidate->GetActorLocation());
				if (!Struck.Contains(Candidate) && Dist < BestDist)
				{
					BestDist = Dist;
					Best = Candidate;
				}
			}
			if (!Best)
			{
				break;
			}
			Struck.Add(Best);
			From = Best->GetActorLocation();
			UGameplayStatics::ApplyDamage(Best, Damage, Instigator, Owner, UDamageType::StaticClass());
		}
		if (Struck.Num() == 0)
		{
			return false;
		}
		break;
	}
	}

	Spend(Selected, Rank);
	OnElementCast.Broadcast(Selected, Origin);
	return true;
}

bool UElementPowerComponent::CastInVehicle(AAethraVehicle* Vehicle)
{
	int32 Rank = 1;
	if (!Vehicle || !Vehicle->bPowersAllowed || !CanCast(Vehicle->GetPlayerState<AAethraPlayerState>(), Selected, Rank))
	{
		return false;
	}
	if (Selected == EAethraElement::Air)
	{
		Vehicle->Boost(1.6f + 0.25f * Rank);
	}
	else if (Selected == EAethraElement::Ice)
	{
		const float RadiusCm = (22.f + 3.f * Rank) * 100.f;
		const TArray<TEnumAsByte<EObjectTypeQuery>> Types = { UEngineTypes::ConvertToObjectType(ECC_PhysicsBody), UEngineTypes::ConvertToObjectType(ECC_Vehicle) };
		TArray<AActor*> Found;
		UKismetSystemLibrary::SphereOverlapActors(this, Vehicle->GetActorLocation(), RadiusCm, Types, AAethraVehicle::StaticClass(), { Vehicle }, Found);
		for (AActor* A : Found)
		{
			Cast<AAethraVehicle>(A)->Freeze(1.4f + 0.2f * Rank);
		}
	}
	else
	{
		return false;
	}
	Spend(Selected, Rank);
	OnElementCast.Broadcast(Selected, Vehicle->GetActorLocation());
	return true;
}
