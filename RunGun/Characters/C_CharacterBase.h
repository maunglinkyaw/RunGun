// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "C_CharacterBase.generated.h"

class UC_AbilitySystemComponent;
class UC_AttributeSet;
class UC_Ability;
class UGameplayEffect;

UCLASS()
class RUNGUN_API AC_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AC_CharacterBase();
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY()
	UC_AttributeSet* m_Attributes;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> m_DefaultAttribute;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UC_Ability>> m_StartingAbilities;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> m_StartingEffects;
	
	UPROPERTY()
	bool m_HasInitAbilities = false;


protected:
	UPROPERTY(BlueprintReadOnly)
	UC_AbilitySystemComponent* m_AbilityComponent;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Implement IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitializeStartingAbilities();

	//virtual void OnReceiveDamage(F_DamageData& inDamage) override;

};
