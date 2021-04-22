// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RunGun/E_GameplayEnum.h"
#include "RunGun/F_GameplayStruct.h"
#include "C_SkillManager.generated.h"

class AC_Skill;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNGUN_API UC_SkillManager : public UActorComponent
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:	
	UC_SkillManager();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills|Pooling")
	TMap<TSubclassOf<AC_Skill>, F_ActorPool> m_PooledSkills;

	void CreateSkillPool(TSubclassOf<AC_Skill> skillClass, int32 poolCount);
	AC_Skill* m_TempPoolSkill;

	template< class T >
	T* GetSkillFromPool(TSubclassOf<AC_Skill> skillClass)
	{
		if (m_PooledSkills.Contains(skillClass))
			return m_PooledSkills[skillClass].GetFromPool<T>();
		else
			return nullptr;
	}

	void ReturnSkillToPool(AC_Skill* returningSkill);

	void UnregisterSkillFromPool(TSubclassOf<AC_Skill> skillClass, int32 poolCount);

};
