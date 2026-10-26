#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireballProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

// Fire element projectile. Give the Blueprint subclass a Niagara flame effect.
UCLASS()
class AETHRA_API AFireballProjectile : public AActor
{
	GENERATED_BODY()

public:
	AFireballProjectile();

	UPROPERTY(VisibleAnywhere, Category = "Fireball") TObjectPtr<USphereComponent> Collision;
	UPROPERTY(VisibleAnywhere, Category = "Fireball") TObjectPtr<UProjectileMovementComponent> Movement;

	// Set by the caster before the projectile is finished spawning.
	float Damage = 26.f;

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
