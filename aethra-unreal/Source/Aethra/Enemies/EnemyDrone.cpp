#include "Enemies/EnemyDrone.h"
#include "Core/AethraTypes.h"
#include "Player/AethraCharacter.h"
#include "Player/AethraPlayerState.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "EngineUtils.h"
#include "GameFramework/FloatingPawnMovement.h"

AEnemyDrone::AEnemyDrone()
{
	PrimaryActorTick.bCanEverTick = true;
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(90.f);
	Collision->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = Collision;
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Collision);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = 900.f;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyDrone::BeginPlay()
{
	Super::BeginPlay();
	Home = GetActorLocation();
	Health = 40.f + Level * 26.f;
}

APawn* AEnemyDrone::FindTarget() const
{
	APawn* Best = nullptr;
	float BestDist = AggroRange;
	for (TActorIterator<AAethraCharacter> It(GetWorld()); It; ++It)
	{
		if (It->IsHidden() || It->Health <= 0.f)
		{
			continue; // driving or knocked out
		}
		const float Dist = FVector::Dist2D(It->GetActorLocation(), GetActorLocation());
		if (Dist < BestDist)
		{
			BestDist = Dist;
			Best = *It;
		}
	}
	return Best;
}

void AEnemyDrone::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (FrozenTime > 0.f)
	{
		FrozenTime -= DeltaSeconds;
		return;
	}
	APawn* Target = FindTarget();
	const FVector Goal = (Target ? Target->GetActorLocation() : Home) + FVector(0, 0, HoverHeight);
	const FVector Offset = Goal - GetActorLocation();
	if (Offset.Size() > (Target ? 900.f : 150.f))
	{
		AddMovementInput(Offset.GetSafeNormal());
	}
	if (Target)
	{
		SetActorRotation((Target->GetActorLocation() - GetActorLocation()).Rotation());
		AttackTimer -= DeltaSeconds;
		if (AttackTimer <= 0.f)
		{
			AttackTimer = AttackInterval - FMath::Min(0.8f, Level * 0.05f);
			OnAttack(Target);
		}
	}
}

float AEnemyDrone::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health <= 0.f)
	{
		return 0.f;
	}
	const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (DamageEvent.DamageTypeClass == UIceDamageType::StaticClass())
	{
		FrozenTime = 3.f;
	}
	if (DamageEvent.DamageTypeClass == UEarthDamageType::StaticClass() && DamageCauser)
	{
		Movement->Velocity = (GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal2D() * 2200.f + FVector(0, 0, 1000.f);
	}
	Health -= Applied;
	if (Health <= 0.f)
	{
		if (AAethraPlayerState* PS = EventInstigator ? EventInstigator->GetPlayerState<AAethraPlayerState>() : nullptr)
		{
			PS->AwardDefeat(Level);
		}
		OnDefeated();
		SetLifeSpan(0.1f);
	}
	return Applied;
}
