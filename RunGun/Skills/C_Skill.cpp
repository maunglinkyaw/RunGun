// Lin & Yin 2020


#include "C_Skill.h"
#include "C_SkillManager.h"
#include "RunGun/Characters/C_CharacterBase.h"

#include "RunGun/RunGun.h"

AC_Skill::AC_Skill()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AC_Skill::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_Skill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_IsActivated)
	{
		EvaluateDuration(DeltaTime);
	}
	if (m_OnCooldown)
	{
		EvaluateCooldown(DeltaTime);
	}
}

void AC_Skill::EvaluateDuration(float deltaTime)
{
	m_SkillTimer += deltaTime;

	if (m_SkillTimer >= m_SkillDuration)
	{
		DeactivateSkill();
	}
}

void AC_Skill::EvaluateCooldown(float deltaTime)
{
	m_CooldownTimer += deltaTime;

	if (m_CooldownTimer >= m_CooldownDuration)
	{
		m_CooldownTimer = 0.f;
		m_OnCooldown = false;
	}
}


void AC_Skill::InitSkill(AActor* owner, AC_Skill* masterSkill)
{
	m_Owner = owner;
	m_OwnerCharacter = Cast<AC_CharacterBase>(owner);
	m_MasterSkill = masterSkill;

	m_DamageBackUp = m_Damage;
}

void AC_Skill::ActivateSkill(FTransform startTransform /*= FTransform::Identity*/)
{
	UE_LOG(LogSkills, Log, TEXT("%s - ActivateSkill - startTransform: %s"), *GetName(), *startTransform.ToHumanReadableString());
	
	if (ActorHasTag("PooledSkill"))
	{
		UE_LOG(LogSkills, Log, TEXT("%s - ActivateSkill - is Pooled Skill"), *GetName());

		SetActorTickEnabled(true);
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
	}

	SetActorTransform(startTransform);

	m_IsActivated = true;
	m_SkillTimer = 0.f;

	OnSkillActivated();
}

void AC_Skill::DeactivateSkill()
{
	UE_LOG(LogSkills, Log, TEXT("%s - DeactivateSkill"), *GetName());
	
	OnSkillDeactivated();

	m_IsActivated = false;
	m_Damage = m_DamageBackUp;

	if (ActorHasTag("PooledSkill") && m_SkillManager)
	{
		m_SkillManager->ReturnSkillToPool(this);
	}
}

void AC_Skill::SlowOwnerMovement(float slowRate, float duration)
{
	if (m_OwnerCharacter)
	{

	}
}

void AC_Skill::StopOwnerMovement(float duration)
{
	if (m_OwnerCharacter)
	{

	}

}

void AC_Skill::SlowOwnerRotation(float slowRate, float duration)
{
	if (m_OwnerCharacter)
	{

	}
}
