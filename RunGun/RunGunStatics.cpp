// Lin & Yin 2020


#include "RunGunStatics.h"
#include "GM_Base.h"
#include "Skills/C_SkillManager.h"

URunGunStatics::URunGunStatics(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

F_ActorPool URunGunStatics::CreateActorPool(const UObject* WorldContextObject, TSubclassOf<AActor> actorClass, int32 count)
{
	F_ActorPool tempPool;

	if (actorClass && count > 0)
	{
		if (UWorld* tempWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			AActor* tempActor;

			for (int i = 0; i < count; i++)
			{
				tempActor = tempWorld->SpawnActor<AActor>(actorClass);

				tempActor->SetActorEnableCollision(false);
				tempActor->SetActorHiddenInGame(true);
				tempActor->SetActorLocation(FVector(0.f, 0.f, -1000.f));

				tempPool.poolClass = actorClass;
				tempPool.availablePool.AddUnique(tempActor);
			}
		}
	}

	return tempPool;
}

AGM_Base* URunGunStatics::GetGMBase(const UObject* WorldContextObject)
{
	if (UWorld* tempWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		return tempWorld->GetAuthGameMode<AGM_Base>();
	}

	return nullptr;
}

UC_SkillManager* URunGunStatics::GetSkillManager(const UObject* WorldContextObject)
{
	if (UWorld* tempWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		AGM_Base* gmBase = tempWorld->GetAuthGameMode<AGM_Base>();

		if (gmBase)
			return gmBase->m_SkillManager;
	}

	return nullptr;
}

