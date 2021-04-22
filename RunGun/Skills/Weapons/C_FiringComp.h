// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "C_WeaponComp.h"
#include "C_FiringComp.generated.h"

class AC_Skill;
class AC_Projectile;
class UC_SkillManager;

UCLASS()
class RUNGUN_API UC_FiringComp : public UC_WeaponComp
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UC_SkillManager* m_SkillManager = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponStats|Pooling")
	TSubclassOf<AC_Skill> m_SkillClass = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponStats|Pooling")
	int32 m_PoolCount = 0;

	void InitWeaponComp(AC_Weapon* weapon) override;

	virtual void RequestFireWeapon(float chargeRate);
	
	virtual void RemoveWeaponComp() override;

protected:
	virtual void FireWeapon(float chargeRate);
	
	AC_Projectile* m_TempProjectile;

	void BeginPoolAmmo();
	AC_Projectile* GetProjectileFromPool();

};
