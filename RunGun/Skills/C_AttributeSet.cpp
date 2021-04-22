// Lin & Yin 2020


#include "C_AttributeSet.h"
#include "Net/UnrealNetwork.h"

UC_AttributeSet::UC_AttributeSet()
{

}

void UC_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UC_AttributeSet, Health);
	DOREPLIFETIME(UC_AttributeSet, MaxHealth);
}

void UC_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UC_AttributeSet, Health, OldValue);
}

void UC_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UC_AttributeSet, MaxHealth, OldValue);
}
