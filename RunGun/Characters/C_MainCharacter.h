// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "C_CharacterBase.h"
#include "RunGun/E_GameplayEnum.h"
#include "RunGun/F_GameplayStruct.h"
#include "C_MainCharacter.generated.h"

class UCameraComponent;
class UBoxComponent;
class USpringArmComponent;
class AC_PlayerController;
class AC_GameplayHUD;
class AC_Weapon;
class AC_PickUp;

// enum class E_WeaponClass : uint8;

UCLASS()
class RUNGUN_API AC_MainCharacter : public AC_CharacterBase
{
	GENERATED_BODY()
public:
	AC_MainCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controls", meta = (AllowPrivateAccess = "true"))
		AC_PlayerController* m_Controller;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controls", meta = (AllowPrivateAccess = "true"))
		FVector m_CursorPosition = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controls", meta = (AllowPrivateAccess = "true"))
		FRotator m_CursorRotation = FRotator::ZeroRotator;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controls", meta = (AllowPrivateAccess = "true"))
		FVector m_DesiredMovement = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	AC_GameplayHUD* m_GameplayHUD = nullptr;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* m_InteractBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	TSubclassOf<AC_Weapon> m_TestWeaponClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	TSubclassOf<AC_Weapon> m_SideTestWeaponClass = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	bool m_IsInputBlocked = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	float m_InputBlockDuration = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	float m_InputBlockTimer = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	bool m_IsPressingFire = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float m_BaseWalkSpeed = 800.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float m_CurrentWalkSpeed = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	F_RunInventory m_Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float m_DodgeForce = 3000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float m_DodgeCooldown = 0.4f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float m_SwapCooldown = 0.4f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	AC_Weapon* m_ActiveWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	AC_Weapon* m_MainWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	AC_Weapon* m_SideWeapon;

// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
// 	TMap<E_WeaponClass, AC_Weapon*> m_Weapons;

	UFUNCTION()
	virtual void InteractBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void InteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void Tick(float DeltaTime) override;

	void RegisterPlayerController(AC_PlayerController* inController);

	void InputMovement(FVector moveDirection);

	void BlockPlayerInput(float duration);
	void EvaluateInputBlock(float deltaTime);
	void UnblockPlayerInput();

	void ToggleMovement(bool canMove);
	bool CanDoAction();
	
	void RegisterPickUp(AC_PickUp* newPickUp);
	void UnregisterPickUp(AC_PickUp* removedPickUp);

	void Interact();

	void StartWeapon();
	void StopWeapon();
	void ReloadWeapon();
	void StopReloadWeapon();

	void AddAmmoToInventory(int32 weaponIndex, int32 ammoCount);
	int32 RequestAmmoFromInventory(int32 weaponIndex, int32 ammoCount);
	int32 m_TempAmmoCount = 0;

	const int32 GetAmmoCount(int32 weaponIndex);

	// Will try to occupy one of the 2 weapon slots, if none are empty, will replace the active weapon.
	void PickUpNewWeapon(AC_Weapon* newWeapon);

	void SwapWeapon();
	void SwapToWeapon(AC_Weapon* newWeapon);

	void MeleeAttack();

	void UseFirstSkill();
	void UseSecondSkill();
	void Dodge();

protected:
	virtual void BeginPlay() override;

};
