// Lin & Yin 2020


#include "C_Weapon.h"
#include "C_TriggerComp.h"
#include "C_FiringComp.h"
#include "Components/StaticMeshComponent.h"

#include "RunGun/Characters/C_MainCharacter.h"
#include "RunGun/Skills/C_Projectile.h"

#include "RunGun/RunGun.h"

AC_Weapon::AC_Weapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_StaticMesh"));
	m_StaticMesh->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	m_StaticMesh->CanCharacterStepUpOn = ECB_No;

	SetRootComponent(m_StaticMesh);

	m_MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("m_MuzzlePoint"));
	m_MuzzlePoint->SetupAttachment(RootComponent);

	m_TriggerComp = CreateDefaultSubobject<UC_TriggerComp>(TEXT("m_TriggerComp"));
	m_FiringComp = CreateDefaultSubobject<UC_FiringComp>(TEXT("m_FiringComp"));
}

void AC_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
	InitWeapon();
}


void AC_Weapon::InitWeapon()
{
	SetWeaponData();

	InitWeaponComps();
}


void AC_Weapon::SetWeaponData()
{
	//m_CooldownDuration = m_FiringRate;
	m_MaxShots = m_BaseShots;
	m_ShotsLeft = m_MaxShots;

	m_AmmoStorage = m_MaxAmmoStorage;
}

void AC_Weapon::InitWeaponComps()
{
	if (m_TriggerComp)
	{
		m_TriggerComp->InitWeaponComp(this);
	}
	if (m_FiringComp)
	{
		m_FiringComp->InitWeaponComp(this);
	}
}


void AC_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

// 	if (m_IsReloading)
// 	{
// 		EvaluateReload(DeltaTime);
// 	}
// 	else if (m_IsPressingFire && !m_OnCooldown)
// 	{
// 		FireWeapon();
// 	}
}


void AC_Weapon::GetPickedUpBy(AC_MainCharacter* owner)
{
	if (owner)
	{
		m_MainCharacter = owner;
		m_GameplayHUD = m_MainCharacter->m_GameplayHUD;

		AttachToActor(m_MainCharacter, FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");
		m_StaticMesh->SetSimulatePhysics(false);
		SetActorEnableCollision(false);

		//PoolProjectiles();
		//FinishReloadWeapon();
	}
}

void AC_Weapon::DropWeapon()
{
	InterruptWeaponFunctions();

	if (m_MainCharacter)
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		m_StaticMesh->SetSimulatePhysics(true);
		SetActorEnableCollision(true);
	}
}


void AC_Weapon::InterruptWeaponFunctions()
{
	if (m_TriggerComp)
	{
		m_TriggerComp->InterruptWeapon();
	}


}

void AC_Weapon::DrawWeapon()
{
	SetActorHiddenInGame(false);
}

void AC_Weapon::HolsterWeapon()
{
	InterruptWeaponFunctions();
	SetActorHiddenInGame(true);
}


void AC_Weapon::AttemptToReload()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, FString::Printf(TEXT("%s - AttemptToReload"), *GetName()));
	UE_LOG(LogWeapons, Log, TEXT("%s - AttemptToReload"), *GetName());

	if (m_TriggerComp)
	{
		m_TriggerComp->AttemptToReload();
	}
}

void AC_Weapon::BeginReloadWeapon()
{
	if (!m_IsReloading)
	{
		m_IsReloading = true;
		m_ReloadTimer = 0.f;
	}
}


void AC_Weapon::FinishReloadWeapon()
{
	UE_LOG(LogWeapons, Log, TEXT("%s - FinishReloadWeapon"), *GetName());

	int32 missingAmmo = m_MaxShots - m_ShotsLeft;
	
	UE_LOG(LogWeapons, Log, TEXT("%s - FinishReloadWeapon - missingAmmo: %d"), *GetName(), missingAmmo);

	// Always attempt to reload to Max
	if (m_IsFullReload)
	{
		if (missingAmmo > m_MaxShots)
			missingAmmo = m_MaxShots;

		m_TempAmmoCount = GetAmmoFromStorage(missingAmmo);

		if (m_TempAmmoCount > 0)
		{
			m_ShotsLeft += m_TempAmmoCount;
		}
		else 
		{
			if (m_ShotsLeft < 1)
				ReportNoMoreAmmo();
			else
				ReportCannotReload();
		}

		m_IsReloading = false;
	}
	// Reload using AmmoPerReload.
	else
	{
		//int32 tempAmmo = FMath::FloorToInt(m_ReloadTimer / m_ReloadDuration);

		if (missingAmmo > m_AmmoPerReload)
			missingAmmo = m_AmmoPerReload;

		m_TempAmmoCount = GetAmmoFromStorage(missingAmmo);

		if (m_TempAmmoCount > 0)
		{
			m_ShotsLeft += m_TempAmmoCount;

			if (m_ShotsLeft >= m_MaxShots)
			{
				m_ShotsLeft = m_MaxShots;
				m_IsReloading = false;
			}
			else
			{
				m_ReloadTimer = 0.f;
			}
		}
		else
		{
			if (m_ShotsLeft < 1)
				ReportNoMoreAmmo();
			else
				ReportCannotReload();

			m_IsReloading = false;
		}
	}

	UE_LOG(LogWeapons, Log, TEXT("%s - FinishReloadWeapon - m_ShotsLeft: %d"), *GetName(), m_ShotsLeft);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("%s - Reloaded: Shots Left = %d"), *GetName(), m_ShotsLeft));
}

int32 AC_Weapon::GetAmmoFromStorage(int32 ammoCount)
{
	UE_LOG(LogWeapons, Log, TEXT("%s - GetAmmoFromStorage - requested: %d"), *GetName(), ammoCount);

	if (ammoCount < 0 || m_AmmoStorage <= 0)
		return 0;

	if (m_AmmoStorage < ammoCount)
		ammoCount = m_AmmoStorage;

	m_AmmoStorage -= ammoCount;
	
	UE_LOG(LogWeapons, Log, TEXT("%s - GetAmmoFromStorage - returned: %d"), *GetName(), ammoCount);

	return ammoCount;
}

void AC_Weapon::StopReloadWeapon()
{
// 	if (m_IsReloading)
// 	{
// 		m_IsReloading = false;
// 	}
}


void AC_Weapon::StartWeapon()
{
	UE_LOG(LogWeapons, Log, TEXT("%s - StartWeapon"), *GetName());
	
	if (m_TriggerComp)
	{
		m_TriggerComp->StartWeapon();
	}
}

void AC_Weapon::StopWeapon()
{
	if (m_TriggerComp)
	{
		m_TriggerComp->StopWeapon();
	}
}

bool AC_Weapon::DoWeHaveAmmo()
{
	if (m_ShotsLeft < 1)
	{
		AttemptToReload();
		return false;
	}

	UE_LOG(LogWeapons, Log, TEXT("%s - DoWeHaveAmmo - m_ShotsLeft: %d"), *GetName(), m_ShotsLeft);
	return true;
}

void AC_Weapon::ReportCannotReload()
{
	UE_LOG(LogWeapons, Warning, TEXT("%s - ReportCannotReload"), *GetName());

}

void AC_Weapon::ReportNoMoreAmmo()
{
	UE_LOG(LogWeapons, Warning, TEXT("%s - ReportNoMoreAmmo"), *GetName());

}

void AC_Weapon::AddAmmoToStorage(int32 ammoCount)
{
	m_AmmoStorage += ammoCount;

	if (m_AmmoStorage > m_MaxAmmoStorage)
		m_AmmoStorage = m_MaxAmmoStorage;
}

void AC_Weapon::FireWeapon(float chargeRate)
{
	UE_LOG(LogWeapons, Log, TEXT("%s - FireWeapon"), *GetName());

	if (m_FiringComp)
		m_FiringComp->RequestFireWeapon(chargeRate);
	
// 	// Check if we have ammo.
// 	if (DoWeHaveAmmo())
// 	{
// 		m_TempProjectile = GetProjectileFromPool();
// 		UE_LOG(LogWeapons, Log, TEXT("%s - FireWeapon: %s"), *GetName(), *GetNameSafe(m_TempProjectile));
// 
// 		if (m_TempProjectile)
// 		{
// 			FTransform tempTransform = FTransform(m_MuzzlePoint->GetComponentRotation(), m_MuzzlePoint->GetComponentLocation());
// 			m_TempProjectile->InitProjectile(m_ShotSpeed, m_ShotDuration);
// 			m_TempProjectile->ActivateSkill(tempTransform);
// 
// 			m_ShotsLeft--;
// 			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("%s - Weapon Fired - Shots Left: %d"), *GetName(), m_ShotsLeft));
// 		}
// 		else
// 		{
// 			UE_LOG(LogWeapons, Error, TEXT("%s - FireWeapon - NO MORE AMMO IN POOL"), *GetName());
// 			ReportNoMoreAmmo();
// 		}
// 	}
// 
// 	// Regardless of whether the shot succeeded, put this attempt on Cooldown.
// 	//m_OnCooldown = true;
}
