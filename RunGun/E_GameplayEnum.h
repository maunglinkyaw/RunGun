// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"


//	Example usage
//	EnumToString<EVictoryEnum>("EVictoryEnum", VictoryEnum)));
//	FString EnumString = EnumToString(EVictoryEnum, EVictoryEnum::VE_Dance);
template<typename TEnum>
static FORCEINLINE FString EnumToString(const FString& Name, TEnum Value)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr)
	{
		return FString("Invalid");
	}
	return enumPtr->GetNameByValue((int64)Value).ToString();
}

#define EnumToString(EnumClassName, ValueOfEnum) EnumToString<EnumClassName>(FString(TEXT(#EnumClassName)), (ValueOfEnum))

UENUM(BlueprintType)
enum class E_WeaponClass : uint8
{
	MAIN_WEAPON = 0		UMETA(DisplayName = "Main Weapon"),
	SIDE_WEAPON = 1		UMETA(DisplayName = "Side Weapon"),
};

UENUM(BlueprintType)
enum class E_Alliance : uint8
{
	FRIENDLY,
	NEUTRAL,
	HOSTILE,
};

UENUM(BlueprintType)
enum class E_AbilityInput : uint8
{
	None,
	Confirm,
	Cancel,
	MainAttack,
	MeleeAttack,
};