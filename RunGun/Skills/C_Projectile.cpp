// Lin & Yin 2020


#include "C_Projectile.h"
#include "C_SkillManager.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Weapons/C_Weapon.h"
#include "RunGun/RunGun.h"


AC_Projectile::AC_Projectile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Use a sphere as a simple collision representation
	m_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("m_Collision"));
	m_Collision->InitSphereRadius(10.f);
	m_Collision->BodyInstance.SetCollisionProfileName("Projectile");
	m_Collision->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	m_Collision->CanCharacterStepUpOn = ECB_No;

	// set up a notification for when this component hits something blocking
	m_Collision->OnComponentBeginOverlap.AddDynamic(this, &AC_Projectile::OnBeginOverlap);
	// Set as root component
	SetRootComponent(m_Collision);

	m_ProjectilePSC = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("m_PSC"));
	m_ProjectilePSC->SetupAttachment(RootComponent);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("m_ProjectileMovement"));
	//m_ProjectileMovement->UpdatedComponent = m_Collision;
	m_ProjectileMovement->bAutoRegisterUpdatedComponent = false;
	m_ProjectileMovement->InitialSpeed = 0.f;
	m_ProjectileMovement->MaxSpeed = 8000.f;
	m_ProjectileMovement->bRotationFollowsVelocity = true;
	m_ProjectileMovement->bShouldBounce = false;
	m_ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AC_Projectile::BeginPlay()
{
	Super::BeginPlay();

}

void AC_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AC_Projectile::InitProjectile(float startVelocity, float lifetime)
{
	UE_LOG(LogWeapons, Log, TEXT("%s - InitProjectile - startVelocity: %f, lifetime: %f"), *GetName(), startVelocity, lifetime);

	m_StartVelocity = startVelocity;
	m_SkillDuration = lifetime;
}

void AC_Projectile::OnSkillActivated()
{
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);

	m_ProjectileMovement->SetUpdatedComponent(m_Collision);
	m_ProjectileMovement->SetVelocityInLocalSpace(FVector(m_StartVelocity, 0.f, 0.f));

	m_ProjectilePSC->ActivateSystem();
	m_ProjectilePSC->SetHiddenInGame(false);
}

void AC_Projectile::OnSkillDeactivated()
{
	FHitResult tempHitResult;
	m_ProjectileMovement->StopSimulating(tempHitResult);

	m_ProjectilePSC->DeactivateSystem();
	m_ProjectilePSC->SetHiddenInGame(true);

}

void AC_Projectile::ActivateProjectile(FTransform startTransform, float startVelocity, float lifetime)
{
	UE_LOG(LogSkills, Log, TEXT("%s - ActivateProjectile: %s"), *GetName(), *startTransform.ToHumanReadableString());

	m_IsActive = true;

	m_Lifetime = lifetime;
	m_Timer = 0.f;

	SetActorTransform(startTransform);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);

	m_ProjectileMovement->SetUpdatedComponent(m_Collision);
	m_ProjectileMovement->SetVelocityInLocalSpace(FVector(startVelocity, 0.f, 0.f));

	m_ProjectilePSC->ActivateSystem();
	m_ProjectilePSC->SetHiddenInGame(false);
}

void AC_Projectile::DeactivateProjectile()
{
	UE_LOG(LogSkills, Log, TEXT("%s - DeactivateProjectile: %s, m_Timer: %f"), *GetName(), *GetNameSafe(m_MasterSkill), m_Timer);

	m_IsActive = false;
	m_Timer = 0.f;

	FHitResult tempHitResult;
	m_ProjectileMovement->StopSimulating(tempHitResult);

	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
	SetActorLocation(FVector(0.f, 0.f, -1000.f));

	m_ProjectilePSC->DeactivateSystem();
	m_ProjectilePSC->SetHiddenInGame(true);

	if (m_SkillManager)
	{
		m_SkillManager->ReturnSkillToPool(this);
	}
}

void AC_Projectile::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, FString::Printf(TEXT("OnBeginOverlap: %s - SUCCESS!"), *GetNameSafe(otherActor)));

// 	if ((m_IsActive) && (otherActor) && (otherActor != m_Owner))
// 	{
// 		if (IC_DamageInterface* dmgInterface = Cast<IC_DamageInterface>(otherActor))
// 		{
// 			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("OnBeginOverlap: %s - SUCCESS!"), *GetNameSafe(otherActor)));
// 
// 			dmgInterface->OnReceiveDamage(m_DamageStruct, sweepResult.ImpactPoint, sweepResult.ImpactNormal);
// 
// 			DeactivateProjectile();
// 		}
// 	}

}

