// Lin & Yin 2020


#include "C_SkillManager.h"
#include "C_Skill.h"


#include "RunGun/RunGun.h"

UC_SkillManager::UC_SkillManager()
{
	PrimaryComponentTick.bCanEverTick = false;


}

void UC_SkillManager::BeginPlay()
{
	Super::BeginPlay();


}


void UC_SkillManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}



void UC_SkillManager::CreateSkillPool(TSubclassOf<AC_Skill> skillClass, int32 poolCount)
{
	UE_LOG(LogSkills, Log, TEXT("%s - CreateSkillPool: %s - count: %d"), *GetName(), *GetNameSafe(skillClass), poolCount);

	if (skillClass && poolCount > 0)
	{
		for (int i = 0; i < poolCount; i++)
		{
			m_TempPoolSkill = GetWorld()->SpawnActor<AC_Skill>(skillClass);

			m_TempPoolSkill->Tags.Add("PooledSkill");
			m_TempPoolSkill->m_SkillManager = this;
			//m_TempPoolSkill->InitSkill(m_Owner, this);
			m_TempPoolSkill->SetActorHiddenInGame(true);
			m_TempPoolSkill->SetActorEnableCollision(false);
			m_TempPoolSkill->SetActorLocation(FVector(0.f, 0.f, -1000.f));

			if (m_PooledSkills.Contains(skillClass))
			{
				m_PooledSkills[skillClass].ReturnToPool(m_TempPoolSkill);
			}
			else
			{
				F_ActorPool tempPool;
				tempPool.poolClass = skillClass;
				tempPool.availablePool.AddUnique(m_TempPoolSkill);

				m_PooledSkills.Emplace(skillClass, tempPool);
			}
		}
	}
}

void UC_SkillManager::ReturnSkillToPool(AC_Skill* returningSkill)
{
	UE_LOG(LogSkills, Log, TEXT("%s - ReturnSkillToPool: %s"), *GetName(), *GetNameSafe(returningSkill));

	if (returningSkill && m_PooledSkills.Contains(returningSkill->GetClass()))
	{
		returningSkill->SetActorTickEnabled(false);
		returningSkill->SetActorHiddenInGame(true);
		returningSkill->SetActorEnableCollision(false);
		returningSkill->SetActorLocation(FVector(0.f, 0.f, -1000.f));

		m_PooledSkills[returningSkill->GetClass()].ReturnToPool(returningSkill);
	}
	else
	{
		UE_LOG(LogSkills, Error, TEXT("%s - ReturnSkillToPool: NO POOL EXISTS"), *GetName());
	}

}

void UC_SkillManager::UnregisterSkillFromPool(TSubclassOf<AC_Skill> skillClass, int32 poolCount)
{
	UE_LOG(LogSkills, Log, TEXT("%s - UnregisterSkillFromPool: %s - count: %d"), *GetName(), *GetNameSafe(skillClass), poolCount);

	if (m_PooledSkills.Contains(skillClass))
	{
		AActor* tempActor = nullptr;

		for (int i = 0; i < poolCount; i++)
		{
			tempActor = m_PooledSkills[skillClass].GetFromPool();
			
			UE_LOG(LogSkills, Log, TEXT("%s - UnregisterSkillFromPool - count: %d, destroying: %s"), *GetName(), i, *GetNameSafe(tempActor));

			if (tempActor)
			{
				tempActor->Destroy();
			}
		}
	}
}

