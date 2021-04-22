// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_WeaponComp.generated.h"

class AC_Weapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUNGUN_API UC_WeaponComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AC_Weapon* m_Weapon = nullptr;

	UC_WeaponComp();

	virtual void InitWeaponComp(AC_Weapon* weapon);

	virtual void RemoveWeaponComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void TickWeaponComp(float deltaTime) {};
};
