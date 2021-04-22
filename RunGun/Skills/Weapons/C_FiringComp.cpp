// Lin & Yin 2020


#include "C_FiringComp.h"
#include "C_Weapon.h"

#include "RunGun/Skills/C_Projectile.h"
#include "RunGun/Skills/C_SkillManager.h"

#include "RunGun/RunGun.h"
#include "RunGun/RunGunStatics.h"

void UC_FiringComp::InitWeaponComp(AC_Weapon* weapon)
{
	Super::InitWeaponComp(weapon);

	m_SkillManager = URunGunStatics::GetSkillManager(GetWorld());

	BeginPoolAmmo();
}

void UC_FiringComp::BeginPoolAmmo()
{
	if (m_SkillManager)
		m_SkillManager->CreateSkillPool(m_SkillClass, m_PoolCount);
}

void UC_FiringComp::RequestFireWeapon(float chargeRate)
{
	if (m_Weapon)
		FireWeapon(chargeRate);
}

void UC_FiringComp::RemoveWeaponComp()
{
	UE_LOG(LogWeapons, Log, TEXT("%s - RemoveWeaponComp"), *GetName());

	if (m_SkillManager && m_SkillClass)
		m_SkillManager->UnregisterSkillFromPool(m_SkillClass, m_PoolCount);
	
	Super::RemoveWeaponComp();
}

void UC_FiringComp::FireWeapon(float chargeRate)
{
	m_TempProjectile = GetProjectileFromPool();
	UE_LOG(LogWeapons, Log, TEXT("%s - FireWeapon: %s"), *GetName(), *GetNameSafe(m_TempProjectile));

	if (m_TempProjectile)
	{
		FTransform tempTransform = FTransform(m_Weapon->m_MuzzlePoint->GetComponentRotation(), m_Weapon->m_MuzzlePoint->GetComponentLocation());
		m_TempProjectile->InitProjectile(m_Weapon->m_ShotSpeed + m_Weapon->m_ShotSpeedBonus * chargeRate, m_Weapon->m_ShotDuration);
		m_TempProjectile->ActivateSkill(tempTransform);

		m_Weapon->m_ShotsLeft--;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("%s - Weapon Fired - Shots Left: %d"), *GetName(), m_Weapon->m_ShotsLeft));
	}
	else
	{
		UE_LOG(LogWeapons, Error, TEXT("%s - FireWeapon - NO MORE AMMO IN POOL"), *GetName());
	}

}

AC_Projectile* UC_FiringComp::GetProjectileFromPool()
{
	if (!m_SkillManager)
		return nullptr;
	else
	{
		return m_SkillManager->GetSkillFromPool<AC_Projectile>(m_SkillClass);
	}
}
