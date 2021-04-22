#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_Base.generated.h"

/**
 * 
 */
UCLASS()
class RUNGUN_API AGM_Base : public AGameModeBase
{
	GENERATED_BODY()
public:
	AGM_Base();

 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UC_SkillManager* m_SkillManager = nullptr;

};
