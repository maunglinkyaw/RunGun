// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "F_GameplayStruct.h"
#include "RunGunStatics.generated.h"

class AGM_Base;

UCLASS()
class RUNGUN_API URunGunStatics : public UBlueprintFunctionLibrary
{
GENERATED_UCLASS_BODY()

	UFUNCTION()
	static F_ActorPool CreateActorPool(const UObject* WorldContextObject, TSubclassOf<AActor> actorClass, int32 count);

	UFUNCTION()
	static AGM_Base* GetGMBase(const UObject* WorldContextObject);
	
	UFUNCTION()
	static UC_SkillManager* GetSkillManager(const UObject* WorldContextObject);

};
