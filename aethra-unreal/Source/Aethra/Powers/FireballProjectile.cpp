#include "Powers/FireballProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AFireballProjectile::AFireballProjectile()
{
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(25.f);
	Collision->SetCollisionProfileName(TEXT("Projectile"));
	Collision->OnComponentHit.AddDynamic(this, &AFireballProjectile::OnHit);
	RootComponent = Collision;

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 4800.f;
	Movement->MaxSpeed = 4800.f;
	Movement->ProjectileGravityScale = 0.f;
	Movement->bRotationFollowsVelocity = true;

	InitialLifeSpan = 1.6f;
}

void AFireballProjectile::OnHit(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, FVector, const FHitResult&)
{
	if (OtherActor && OtherActor != GetOwner() && OtherActor != GetInstigator())
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
	}
	Destroy();
}
