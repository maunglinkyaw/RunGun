// Lin & Yin 2020

#include "CW_Bow.h"
#include "C_ChargeTrigger.h"


#include "RunGun/Characters/C_MainCharacter.h"
#include "RunGun/Skills/C_Projectile.h"

#include "RunGun/RunGun.h"

ACW_Bow::ACW_Bow(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UC_ChargeTrigger>(TEXT("m_TriggerComp")))
	//: Super(ObjectInitializer.DoNotCreateDefaultSubobject(TEXT("m_TriggerComp")))
{

}

// void ACW_Bow::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// 
// 	if (m_IsHoldingCharge)
// 	{
// 		EvaluateCharging(DeltaTime);
// 	}
// }
// 
// void ACW_Bow::StartWeapon()
// {
// 	if (DoWeHaveAmmo())
// 	{
// 		m_IsPressingFire = true;   
// 		m_IsHoldingCharge = true;
// 	}
// }
// 
// void ACW_Bow::StopWeapon()
// {
// 	if (m_IsPressingFire)
// 	{
// 		m_IsPressingFire = false;
// 	}
// }

bool ACW_Bow::DoWeHaveAmmo()
{
	UE_LOG(LogWeapons, Log, TEXT("%s - DoWeHaveAmmo - m_ShotsLeft: %d"), *GetName(), m_ShotsLeft);

	if (m_ShotsLeft < 1)
	{
		if (m_AmmoStorage > 0)
		{
			m_ShotsLeft = GetAmmoFromStorage(1);
		}
		else
		{
			ReportNoMoreAmmo();
			return false;
		}
	}

	return true;
}

void ACW_Bow::FireWeapon(float chargeRate)
{
	Super::FireWeapon(chargeRate);

// 	m_TempProjectile = GetProjectileFromPool();
// 	UE_LOG(LogWeapons, Log, TEXT("%s - FireWeapon: %s"), *GetName(), *GetNameSafe(m_TempProjectile));
// 
// 	if (m_TempProjectile)
// 	{
// 		float tempChargeDuration = m_ChargeTimer;
// 		float tempSpeed = m_BaseSpeed;
// 
// 		if (tempChargeDuration > m_MaxChargeDuration)
// 			tempChargeDuration = m_MaxChargeDuration;
// 
// 		tempSpeed = m_BaseSpeed + (tempChargeDuration * m_SpeedPerSecond);
// 
// 		FTransform tempTransform = FTransform(m_MuzzlePoint->GetComponentRotation(), m_MuzzlePoint->GetComponentLocation());
// 		m_TempProjectile->InitProjectile(tempSpeed, m_ShotDuration);
// 		m_TempProjectile->ActivateSkill(tempTransform);
// 
// 		m_ShotsLeft--;
// 		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("%s - Weapon Fired - Shots Left: %d"), *GetName(), m_ShotsLeft));
// 	}
// 	else
// 	{
// 		UE_LOG(LogWeapons, Error, TEXT("%s - FireWeapon - NO MORE AMMO IN POOL"), *GetName());
// 		ReportNoMoreAmmo();
// 	}
}

