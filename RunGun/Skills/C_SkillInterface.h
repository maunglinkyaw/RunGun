// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RunGun/F_GameplayStruct.h"
#include "C_SkillInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UC_SkillInterface : public UInterface
{
	GENERATED_BODY()
};

class RUNGUN_API IC_SkillInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void OnReceiveDamage(F_DamageData& inDamage) {};
};
