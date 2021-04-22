// Lin & Yin 2020

#include "C_CharacterBase.h"
#include "RunGun/Skills/Weapons/C_Weapon.h"
#include "RunGun/Skills/C_AbilitySystemComponent.h"
#include "RunGun/Skills/C_AttributeSet.h"
#include "RunGun/Skills/C_Ability.h"

#include <GameplayEffectTypes.h>


AC_CharacterBase::AC_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	m_AbilityComponent = CreateDefaultSubobject<UC_AbilitySystemComponent>(TEXT("m_AbilityComponent"));
	m_AbilityComponent->SetIsReplicated(true);
	m_AbilityComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	
	m_Attributes = CreateDefaultSubobject<UC_AttributeSet>(TEXT("m_Attributes"));

}

void AC_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Try setting the inventory source, this will fail for AI
// 	InventorySource = NewController;
// 
// 	if (InventorySource)
// 	{
// 		InventoryUpdateHandle = InventorySource->GetSlottedItemChangedDelegate().AddUObject(this, &ARPGCharacterBase::OnItemSlotChanged);
// 		InventoryLoadedHandle = InventorySource->GetInventoryLoadedDelegate().AddUObject(this, &ARPGCharacterBase::RefreshSlottedGameplayAbilities);
// 	}

	if (m_AbilityComponent)
	{
		m_AbilityComponent->InitAbilityActorInfo(this, this);
		InitializeStartingAbilities();
	}
}

void AC_CharacterBase::UnPossessed()
{
	Super::UnPossessed();
	
}

void AC_CharacterBase::OnRep_Controller()
{
	Super::OnRep_Controller();

	// Our controller changed, must update ActorInfo on AbilitySystemComponent
// 	if (m_AbilityComponent)
// 	{
// 		m_AbilityComponent->RefreshAbilityActorInfo();
// 	}
}

void AC_CharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (m_AbilityComponent)
	{
		m_AbilityComponent->InitAbilityActorInfo(this, this);

		if (InputComponent)
		{
			const FGameplayAbilityInputBinds inputBinds("Confirm", "Cancel", "E_AbilityInput", static_cast<int32>(E_AbilityInput::Confirm), static_cast<int32>(E_AbilityInput::Cancel));
			m_AbilityComponent->BindAbilityActivationToInputComponent(InputComponent, inputBinds);
		}
	}
}

void AC_CharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void AC_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AC_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (InputComponent && m_AbilityComponent)
	{
		const FGameplayAbilityInputBinds inputBinds("Confirm", "Cancel", "E_AbilityInput", static_cast<int32>(E_AbilityInput::Confirm), static_cast<int32>(E_AbilityInput::Cancel));
		m_AbilityComponent->BindAbilityActivationToInputComponent(InputComponent, inputBinds);
	}
}

UAbilitySystemComponent* AC_CharacterBase::GetAbilitySystemComponent() const
{
	return m_AbilityComponent;
}

void AC_CharacterBase::InitializeStartingAbilities()
{
	if (m_AbilityComponent && HasAuthority() && !m_HasInitAbilities)
	{
		if (m_DefaultAttribute)
		{
			FGameplayEffectContextHandle EffectContext = m_AbilityComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			
			FGameplayEffectSpecHandle NewHandle = m_AbilityComponent->MakeOutgoingSpec(m_DefaultAttribute, 1, EffectContext);
			
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = m_AbilityComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), m_AbilityComponent);
			}
		}

		for (TSubclassOf<UC_Ability>& startAbility : m_StartingAbilities)
		{
			m_AbilityComponent->GiveAbility(
			FGameplayAbilitySpec(startAbility, 1, static_cast<int32>(startAbility.GetDefaultObject()->m_AbilityInput), this));
		}

		for (TSubclassOf<UGameplayEffect>& GameplayEffect : m_StartingEffects)
		{
// 			FGameplayEffectContextHandle EffectContext = m_AbilityComponent->MakeEffectContext();
// 			EffectContext.AddSourceObject(this);
// 
// 			FGameplayEffectSpecHandle NewHandle = m_AbilityComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
// 			if (NewHandle.IsValid())
// 			{
// 				FActiveGameplayEffectHandle ActiveGEHandle = m_AbilityComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), m_AbilityComponent);
// 			}
		}
		


		m_HasInitAbilities = true;
	}
}
