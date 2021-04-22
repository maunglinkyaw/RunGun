// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "C_WeaponComp.h"
#include "C_TriggerComp.generated.h"

/**
 * 
 */
UCLASS()
class RUNGUN_API UC_TriggerComp : public UC_WeaponComp
{
	GENERATED_BODY()

public:
	UC_TriggerComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void InitWeaponComp(AC_Weapon* weapon) override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "WeaponStats|Debug")
	float m_CooldownTimer = 0.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "WeaponStats|Debug")
	bool m_OnCooldown = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "WeaponStats|Debug")
	bool m_IsPressingFire = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "WeaponStats|Debug")
	bool m_IsReloading = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "WeaponStats|Debug")
	float m_ReloadTimer = 0.f;

	virtual void StartWeapon();
	virtual void StopWeapon();
	
	// Only applicable for Release to Fire weapons that can be cancelled (like Bows).
	virtual void UnreadyWeapon();

	virtual bool DoWeHaveAmmo();
	virtual void AttemptToReload();
	void CancelReloadWeapon();

	void InterruptWeapon();

protected:
	virtual void AttemptFireWeapon();

	void BeginReloadWeapon();
	void EvaluateCooldown(float deltaTime);
	void EvaluateReload(float deltaTime);
	void FinishReloadWeapon();

};
