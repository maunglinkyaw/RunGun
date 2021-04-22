// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RunGun/E_GameplayEnum.h"
#include "C_Ability.generated.h"

/**
 * 
 */
UCLASS()
class RUNGUN_API UC_Ability : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UC_Ability();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	E_AbilityInput m_AbilityInput = E_AbilityInput::None;
};
