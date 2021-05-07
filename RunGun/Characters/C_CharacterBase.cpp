// Lin & Yin 2020

#include "C_CharacterBase.h"
#include "RunGun/Skills/Weapons/C_Weapon.h"


AC_CharacterBase::AC_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
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
}

void AC_CharacterBase::UnPossessed()
{
	Super::UnPossessed();
	
}

void AC_CharacterBase::OnRep_Controller()
{
	Super::OnRep_Controller();
}

void AC_CharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
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

}
