// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "C_Skill.h"
#include "C_Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class RUNGUN_API AC_Projectile : public AC_Skill
{
	GENERATED_BODY()

public:
	AC_Projectile();

	virtual void Tick(float deltaTime) override;

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, Category = Projectile)
		USphereComponent* m_Collision;
	UPROPERTY(VisibleAnywhere, Category = Projectile)
		UParticleSystemComponent* m_ProjectilePSC;
	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* m_ProjectileMovement;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
	float m_StartVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
	float m_Lifetime = 1.5f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Projectile)
	bool m_IsActive = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Projectile)
	float m_Timer = 0.f;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	void InitProjectile(float startVelocity, float lifetime);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ActivateProjectile(FTransform startTransform, float startVelocity, float lifetime);
	virtual void DeactivateProjectile();

	virtual void OnSkillActivated() override;
	virtual void OnSkillDeactivated() override;


};
