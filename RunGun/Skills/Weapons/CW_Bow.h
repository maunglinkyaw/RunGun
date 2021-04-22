// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "C_Weapon.h"
#include "CW_Bow.generated.h"

UCLASS()
class RUNGUN_API ACW_Bow : public AC_Weapon
{
	GENERATED_BODY()
public:
	ACW_Bow(const FObjectInitializer& ObjectInitializer);

// 	virtual void Tick(float DeltaTime) override;
// 
// 	virtual void StartWeapon() override;
// 	virtual void StopWeapon() override;

// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
// 	float m_MinChargeDuration = 0.15f;
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
// 	float m_DamagePerSecond = 10.f;  
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
// 	float m_MaxChargeDuration = 1.f;
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
// 	float m_BaseSpeed = 1000.f;
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
// 	float m_SpeedPerSecond = 2000.f;
// 
// 	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "WeaponStats|Debug")
// 	bool m_IsHoldingCharge = false;
// 	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "WeaponStats|Debug")
// 	float m_ChargeTimer = 0.f;

	// Cancels the charge without firing.
	void UnreadyWeapon();

protected:
	virtual void FireWeapon(float chargeRate) override;

	virtual bool DoWeHaveAmmo() override;

};
