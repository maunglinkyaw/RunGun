// Lin & Yin 2020


#include "C_TriggerComp.h"
#include "C_Weapon.h"

#include "RunGun/RunGun.h"

UC_TriggerComp::UC_TriggerComp()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UC_TriggerComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (m_OnCooldown)
	{
		EvaluateCooldown(DeltaTime);
	}
	else if (m_IsReloading)
	{
		EvaluateReload(DeltaTime);
	}
	else if (m_IsPressingFire)
	{
		AttemptFireWeapon();
	}
}

void UC_TriggerComp::EvaluateCooldown(float deltaTime)
{
	m_CooldownTimer += deltaTime;

	if (m_CooldownTimer >= m_Weapon->m_FiringRate)
	{
		m_CooldownTimer = 0.f;
		m_OnCooldown = false;
	}
}

void UC_TriggerComp::EvaluateReload(float deltaTime)
{
	m_ReloadTimer += deltaTime;

	if (m_ReloadTimer >= m_Weapon->m_ReloadDuration)
	{
		FinishReloadWeapon();
	}
}

void UC_TriggerComp::InitWeaponComp(AC_Weapon* weapon)
{
	Super::InitWeaponComp(weapon);
}

void UC_TriggerComp::StartWeapon()
{
	UE_LOG(LogWeapons, Log, TEXT("%s - StartWeapon"), *GetName());

	m_IsPressingFire = true;

	// Also fire the weapon on the same frame we register, if able to fire.
	AttemptFireWeapon();
}

void UC_TriggerComp::StopWeapon()
{
	m_IsPressingFire = false;

}


void UC_TriggerComp::UnreadyWeapon()
{

}

void UC_TriggerComp::AttemptFireWeapon()
{
	if (!m_OnCooldown && !m_IsReloading && DoWeHaveAmmo())
		m_Weapon->FireWeapon();

	m_OnCooldown = true;
}

bool UC_TriggerComp::DoWeHaveAmmo()
{
	if (m_Weapon->m_ShotsLeft < 1)
	{
		AttemptToReload();
		return false;
	}

	UE_LOG(LogWeapons, Log, TEXT("%s - DoWeHaveAmmo - m_ShotsLeft: %d"), *GetName(), m_Weapon->m_ShotsLeft);
	return true;
}


void UC_TriggerComp::AttemptToReload()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, FString::Printf(TEXT("%s - AttemptToReload"), *GetName()));
	UE_LOG(LogWeapons, Log, TEXT("%s - AttemptToReload"), *GetName());

	if (m_IsReloading)
		return;

	if (m_Weapon->m_ShotsLeft >= m_Weapon->m_MaxShots)
	{
		m_Weapon->ReportCannotReload();
		return;
	}

	if (m_Weapon->m_AmmoStorage < 1)
	{
		m_Weapon->ReportNoMoreAmmo();
		return;
	}

	BeginReloadWeapon();
}

void UC_TriggerComp::BeginReloadWeapon()
{
	if (!m_IsReloading)
	{
		m_IsReloading = true;
		m_ReloadTimer = 0.f;
	}
}


void UC_TriggerComp::FinishReloadWeapon()
{
	UE_LOG(LogWeapons, Log, TEXT("%s - FinishReloadWeapon"), *GetName());

	int32 missingAmmo = m_Weapon->m_MaxShots - m_Weapon->m_ShotsLeft;

	UE_LOG(LogWeapons, Log, TEXT("%s - FinishReloadWeapon - missingAmmo: %d"), *GetName(), missingAmmo);

	// Always attempt to reload to Max
	if (m_Weapon->m_IsFullReload)
	{
		if (missingAmmo > m_Weapon->m_MaxShots)
			missingAmmo = m_Weapon->m_MaxShots;

		int32 tempAmmo = m_Weapon->GetAmmoFromStorage(missingAmmo);

		if (tempAmmo > 0)
		{
			m_Weapon->m_ShotsLeft += tempAmmo;
		}
		else
		{
			if (m_Weapon->m_ShotsLeft < 1)
				m_Weapon->ReportNoMoreAmmo();
			else
				m_Weapon->ReportCannotReload();
		}

		m_IsReloading = false;
	}
	// Reload using AmmoPerReload.
	else
	{
		//int32 tempAmmo = FMath::FloorToInt(m_ReloadTimer / m_ReloadDuration);

		if (missingAmmo > m_Weapon->m_AmmoPerReload)
			missingAmmo = m_Weapon->m_AmmoPerReload;

		int32 tempAmmo = m_Weapon->GetAmmoFromStorage(missingAmmo);

		if (tempAmmo > 0)
		{
			m_Weapon->m_ShotsLeft += tempAmmo;

			if (m_Weapon->m_ShotsLeft >= m_Weapon->m_MaxShots)
			{
				m_Weapon->m_ShotsLeft = m_Weapon->m_MaxShots;
				m_IsReloading = false;
			}
			else
			{
				m_ReloadTimer = 0.f;
			}
		}
		else
		{
			if (m_Weapon->m_ShotsLeft < 1)
				m_Weapon->ReportNoMoreAmmo();
			else
				m_Weapon->ReportCannotReload();

			m_IsReloading = false;
		}
	}

	UE_LOG(LogWeapons, Log, TEXT("%s - FinishReloadWeapon - m_ShotsLeft: %d"), *GetName(), m_Weapon->m_ShotsLeft);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("%s - Reloaded: Shots Left = %d"), *GetName(), m_Weapon->m_ShotsLeft));
}

void UC_TriggerComp::CancelReloadWeapon()
{

}

void UC_TriggerComp::InterruptWeapon()
{
	if (m_IsReloading)
	{
		m_IsReloading = false;
	}
	if (m_IsPressingFire)
	{
		m_IsPressingFire = false;
	}

}
