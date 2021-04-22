// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "E_GameplayEnum.h"
#include "F_GameplayStruct.generated.h"


USTRUCT(Blueprintable)
struct RUNGUN_API F_ActorPool
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<AActor> poolClass;
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> availablePool;

	FORCEINLINE F_ActorPool()
	{
		poolClass = nullptr;
	}

public:
	// Pop an Actor from availablePool.
	AActor* GetFromPool()
	{
		if (availablePool.Num() == 0)
			return nullptr;
		else
			return availablePool.Pop();
	}

	template< class T >
	T* GetFromPool()
	{
		if (availablePool.Num() == 0)
			return nullptr;
		else
			return Cast<T>(availablePool.Pop());
	}


	void ReturnToPool(AActor* returningActor)
	{
		if (poolClass == returningActor->GetClass())
		{
			availablePool.AddUnique(returningActor);
		}
	}
};

USTRUCT(Blueprintable)
struct F_DamageData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		float damage = 0.f;
	UPROPERTY(EditAnywhere)
		float knockback = 0.f;
	UPROPERTY(EditAnywhere)
		float stunDuration = 0.f;
// 	UPROPERTY()
// 		FHitResult& hitLocation;

	F_DamageData()
	{

	}
};

USTRUCT(Blueprintable)
struct F_RunInventory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int mainAmmo = 0;
	UPROPERTY(EditAnywhere)
	int maxMainAmmo = 0;
	UPROPERTY(EditAnywhere)
	int sideAmmo = 0;
	UPROPERTY(EditAnywhere)
	int maxSideAmmo = 0;
	UPROPERTY(EditAnywhere)
	int scraps = 0;

	F_RunInventory()
	{

	}
};
