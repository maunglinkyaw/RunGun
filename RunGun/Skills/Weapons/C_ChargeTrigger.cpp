// Lin & Yin 2020


#include "C_ChargeTrigger.h"
#include "C_Weapon.h"

#include "RunGun/RunGun.h"

void UC_ChargeTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_IsHoldingCharge)
	{
		EvaluateCharge(DeltaTime);
	}
}

void UC_ChargeTrigger::InitWeaponComp(AC_Weapon* weapon)
{
	Super::InitWeaponComp(weapon);

}

void UC_ChargeTrigger::StartWeapon()
{
	UE_LOG(LogWeapons, Log, TEXT("%s - StartWeapon"), *GetName());

	m_IsPressingFire = true;

	// Also fire the weapon on the same frame we register, if able to fire.
	AttemptFireWeapon();

	// 	if (DoWeHaveAmmo())
// 	{
// 		m_IsPressingFire = true;
// 		m_IsHoldingCharge = true;
// 	}
}

void UC_ChargeTrigger::StopWeapon()
{
	if (m_IsPressingFire)
	{
		m_IsPressingFire = false;
	}
}

void UC_ChargeTrigger::AttemptFireWeapon()
{
	if (!m_IsHoldingCharge && !m_IsReloading && DoWeHaveAmmo())
		m_IsHoldingCharge = true;
}

void UC_ChargeTrigger::EvaluateCharge(float deltaTime)
{
	m_ChargeTimer += deltaTime;

	if (!m_IsPressingFire && m_ChargeTimer >= m_MinChargeDuration)
	{
		float chargeRatio = FMath::SmoothStep(0.f, m_MaxChargeDuration, m_ChargeTimer);
		
		UE_LOG(LogWeapons, Log, TEXT("%s - EvaluateCharge - Firing - m_ChargeTimer: %f, chargeRatio: %f"), *GetName(), m_ChargeTimer, chargeRatio);

		m_Weapon->FireWeapon(chargeRatio);

		m_IsHoldingCharge = false;
		m_ChargeTimer = 0.f;
	}
}