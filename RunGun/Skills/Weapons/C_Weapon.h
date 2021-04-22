// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunGun/Skills/C_Skill.h"
#include "C_Weapon.generated.h"

class AC_MainCharacter;
class AC_Projectile;
class ACUI_GameplayHUD;

UENUM(BlueprintType)
enum class E_ReloadType : uint8
{
	FULL_RELOAD,
	CHARGE_RELOAD,
	ONE_BY_ONE,
};

class UC_FiringComp;
class UC_TriggerComp;


UCLASS()
class RUNGUN_API AC_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AC_Weapon(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* m_MuzzlePoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* m_StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UC_TriggerComp* m_TriggerComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UC_FiringComp* m_FiringComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AC_MainCharacter* m_MainCharacter = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	ACUI_GameplayHUD* m_GameplayHUD = nullptr;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	AActor* m_Owner = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
	bool m_IsMainWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponStats|Pooling")
	TSubclassOf<AC_Skill> m_SkillClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponStats|Pooling")
	int32 m_PoolCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
	F_DamageData m_WeaponDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
	int32 m_BaseShots = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
	float m_FiringRate = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
	float m_ShotSpeed = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
	float m_ShotSpeedBonus = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
	float m_ShotDuration = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
	float m_SwapCooldown = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats")
	int32 m_MaxAmmoStorage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats|Reload")
	bool m_IsFullReload = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats|Reload", meta = (EditCondition = "!m_IsFullReload"))
	int32 m_AmmoPerReload = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponStats|Reload")
	float m_ReloadDuration = 0.f;

	// Debug Variables
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "WeaponStats|Debug")
	int32 m_MaxShots = 0;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "WeaponStats|Debug")
	int32 m_ShotsLeft = 0;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "WeaponStats")
	int32 m_AmmoStorage = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "WeaponStats|Debug")
	bool m_IsReloading = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "WeaponStats|Debug")
	float m_ReloadTimer = 0.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "WeaponStats|Debug")
	bool m_IsPressingFire = false;

	AC_Projectile* m_TempProjectile;


protected:
	virtual void BeginPlay() override;

	void InitWeapon();

	void SetWeaponData();

	void InitWeaponComps();

	virtual bool DoWeHaveAmmo();
	int32 m_TempAmmoCount;

	void BeginReloadWeapon();
	void FinishReloadWeapon();


public:	
	virtual void Tick(float DeltaTime) override;

	void GetPickedUpBy(AC_MainCharacter* mainCharacter);

	void AddAmmoToStorage(int32 ammoCount);

	void DrawWeapon();
	void HolsterWeapon();
	void DropWeapon();
	void InterruptWeaponFunctions();

	virtual void StartWeapon();
	virtual void StopWeapon();
	
	virtual void AttemptToReload();
	void StopReloadWeapon();

	void ReportCannotReload();
	void ReportNoMoreAmmo();

	int32 GetAmmoFromStorage(int32 ammoCount);

	virtual void FireWeapon(float chargeRate = 1.f);
};
