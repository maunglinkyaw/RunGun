// Lin & Yin 2020


#include "C_WeaponComp.h"
#include "C_Weapon.h"

#include "RunGun/RunGun.h"

UC_WeaponComp::UC_WeaponComp()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

}

void UC_WeaponComp::InitWeaponComp(AC_Weapon* weapon)
{
	UE_LOG(LogWeapons, Log, TEXT("%s - InitWeaponComp - weapon: %s"), *GetName(), *GetNameSafe(weapon));

	m_Weapon = weapon;

	if (PrimaryComponentTick.bCanEverTick)
	{
		SetComponentTickEnabled(true);
	}
}

void UC_WeaponComp::RemoveWeaponComp()
{
	DestroyComponent();
}

void UC_WeaponComp::BeginPlay()
{
	Super::BeginPlay();

	
}


void UC_WeaponComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

