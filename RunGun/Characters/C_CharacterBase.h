// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_CharacterBase.generated.h"


UCLASS()
class RUNGUN_API AC_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AC_CharacterBase();
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	

protected:

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//virtual void OnReceiveDamage(F_DamageData& inDamage) override;

};
