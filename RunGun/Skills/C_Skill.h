// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_SkillInterface.h"
// #include "RunGun/E_GameplayEnum.h"
// #include "RunGun/F_GameplayStruct.h"
#include "C_Skill.generated.h"

class AC_CharacterBase;
class UC_SkillManager;

UCLASS()
class RUNGUN_API AC_Skill : public AActor, public IC_SkillInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_Skill();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills")
	F_DamageData m_Damage;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skills")
	F_DamageData m_DamageBackUp;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	AActor* m_Owner = nullptr;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	AC_CharacterBase* m_OwnerCharacter = nullptr;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	AC_Skill* m_MasterSkill = nullptr;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	UC_SkillManager* m_SkillManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	bool m_OnCooldown = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void InitSkill(AActor* owner, AC_Skill* masterSkill);

	// This modification is reverted upon skill ending.
	void ModifyDamage(F_DamageData tempDamage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	bool m_IsActivated = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills")
	float m_SkillDuration = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills")
	float m_SkillTimer = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills")
	float m_CooldownDuration = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	float m_CooldownTimer = 0.f;

	void EvaluateDuration(float deltaTime);
	void EvaluateCooldown(float deltaTime);

	void SlowOwnerMovement(float slowRate, float duration);
	void StopOwnerMovement(float duration);
	void SlowOwnerRotation(float slowRate, float duration);

	virtual void OnSkillActivated() {};
	virtual void OnSkillDeactivated() {};

public:	

	void ActivateSkill(FTransform startTransform = FTransform::Identity);
	void DeactivateSkill();

};
