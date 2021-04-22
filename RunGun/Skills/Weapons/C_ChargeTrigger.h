// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "C_TriggerComp.h"
#include "C_ChargeTrigger.generated.h"

/**
 * 
 */
UCLASS()
class RUNGUN_API UC_ChargeTrigger : public UC_TriggerComp
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
	float m_MinChargeDuration = 0.15f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
	float m_MaxChargeDuration = 1.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "WeaponStats|Debug")
	bool m_IsHoldingCharge = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "WeaponStats|Debug")
	float m_ChargeTimer = 0.f;
	
	void InitWeaponComp(AC_Weapon* weapon) override;


	virtual void StartWeapon() override;
	virtual void StopWeapon() override;

protected:
	virtual void AttemptFireWeapon() override;

	void EvaluateCharge(float deltaTime);


};
