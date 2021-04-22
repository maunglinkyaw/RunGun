// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "C_PlayerController.generated.h"

class AC_MainCharacter;
class ACUI_GameplayHUD;

UENUM(Blueprintable)
enum class E_InputMode : uint8
{
	INPUT_CHAR = 0				UMETA(DisplayName = "Input Character"),
	INPUT_MENU					UMETA(DisplayName = "Input Menu"),
};

UENUM(Blueprintable)
enum class E_MovementMode : uint8
{
	MOVEMENT_MODE = 0			UMETA(DisplayName = "Movement Mode"),
	AIMING_MODE					UMETA(DisplayName = "Aiming Mode"),
};


UCLASS()
class RUNGUN_API AC_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	AC_MainCharacter* m_MainCharacter = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	E_InputMode m_InputMode = E_InputMode::INPUT_CHAR;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	E_MovementMode m_MovementMode = E_MovementMode::MOVEMENT_MODE;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	ACUI_GameplayHUD* m_GameplayHUD = nullptr;

public:
	AC_PlayerController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
	virtual void SetupInputComponent() override;
	void EvaluateMovementInput(float deltaTime);
	void SpinCharacterToMouse();
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Controls", meta = (AllowPrivateAccess = "true"))
	FVector m_CursorPosition = FVector::ZeroVector;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Controls", meta = (AllowPrivateAccess = "true"))
	FRotator m_CursorRotation = FRotator::ZeroRotator;
	FHitResult m_MouseHitResult;

	UFUNCTION()
		void PressLeftClick();
	UFUNCTION()
		void ReleaseLeftClick();
	UFUNCTION()
		void PressRightClick();
	UFUNCTION()
		void ReleaseRightClick();
	UFUNCTION()
		void PressLeftShoulder();
	UFUNCTION()
		void ReleaseLeftShoulder();
	UFUNCTION()
		void PressRightShoulder();
	UFUNCTION()
		void ReleaseRightShoulder();


	UFUNCTION()
	void ToggleMainAttack(bool isStarting);
	UFUNCTION()
	void ToggleSecondAttack(bool isStarting);

	UFUNCTION()
		void UseFirstSkill();
	UFUNCTION()
		void UseSecondSkill();
	UFUNCTION()
		void ReloadWeapon();
	UFUNCTION()
		void SwapWeapon();

	UFUNCTION()
		void EscapeAction();
	UFUNCTION()
		void CancelAction();
	UFUNCTION()
		void ConfirmAction();

	// Static names for axis bindings
	static const FName m_MoveUpBinding;
	static const FName m_MoveRightBinding;
	static const FName m_AimUpBinding;
	static const FName m_AimRightBinding;
};
