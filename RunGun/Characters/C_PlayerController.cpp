// Lin & Yin 2020


#include "C_PlayerController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "C_MainCharacter.h"

#include "RunGun/RunGun.h"

#include "RunGun/UI/Gameplay/CUI_GameplayHUD.h"

const FName AC_PlayerController::m_MoveUpBinding("MoveUp");
const FName AC_PlayerController::m_MoveRightBinding("MoveRight");
const FName AC_PlayerController::m_AimUpBinding("AimUp");
const FName AC_PlayerController::m_AimRightBinding("AimRight");

AC_PlayerController::AC_PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bAutoManageActiveCameraTarget = true;

// 	ConstructorHelpers::FObjectFinder<UClass> gameplayUIClass(TEXT("Class'/Game/UI/Gameplay/CUI_GameplayHUD_BP.CUI_GameplayHUD_BP_C'"));
// 	if (gameplayUIClass.Succeeded())
// 	{
// 		m_GameplayHUDClass = gameplayUIClass.Object;
// 	}
}

void AC_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	m_MainCharacter = Cast<AC_MainCharacter>(GetPawn());

	if (m_MainCharacter)
	{
		m_MainCharacter->RegisterPlayerController(this);
		//SetViewTarget(m_MainCharacter);
	}

	m_GameplayHUD = Cast<ACUI_GameplayHUD>(MyHUD);

	UE_LOG(LogUI, Log, TEXT("%s - BeginPlay - m_GameHUD: %s"), *GetName(), *GetNameSafe(m_GameplayHUD));
}

void AC_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_InputMode == E_InputMode::INPUT_CHAR)
	{
		switch (m_MovementMode)
		{
		case E_MovementMode::MOVEMENT_MODE:
			EvaluateMovementInput(DeltaTime);
			break;

		case E_MovementMode::AIMING_MODE:
			SpinCharacterToMouse();
			break;

		}
	}
}


#pragma region PlayerInput

void AC_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(m_MoveUpBinding);
	InputComponent->BindAxis(m_MoveRightBinding);
	InputComponent->BindAxis(m_AimUpBinding);
	InputComponent->BindAxis(m_AimRightBinding);

	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AC_PlayerController::PressLeftClick);
	InputComponent->BindAction("LeftClick", IE_Released, this, &AC_PlayerController::ReleaseLeftClick);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AC_PlayerController::PressRightClick);
	InputComponent->BindAction("RightClick", IE_Released, this, &AC_PlayerController::ReleaseRightClick);

	InputComponent->BindAction("LeftShoulder", IE_Pressed, this, &AC_PlayerController::PressLeftClick);
	InputComponent->BindAction("LeftShoulder", IE_Released, this, &AC_PlayerController::ReleaseLeftClick);
	InputComponent->BindAction("RightShoulder", IE_Pressed, this, &AC_PlayerController::PressRightClick);
	InputComponent->BindAction("RightShoulder", IE_Released, this, &AC_PlayerController::ReleaseRightClick);

	InputComponent->BindAction("Escape", IE_Pressed, this, &AC_PlayerController::EscapeAction);

	InputComponent->BindAction("ConfirmAction", IE_Pressed, this, &AC_PlayerController::ConfirmAction);
	InputComponent->BindAction("CancelAction", IE_Pressed, this, &AC_PlayerController::CancelAction);

	InputComponent->BindAction("Reload", IE_Pressed, this, &AC_PlayerController::ReloadWeapon);
	InputComponent->BindAction("SwapWeapon", IE_Pressed, this, &AC_PlayerController::SwapWeapon);

	InputComponent->BindAction("UseFirstSkill", IE_Pressed, this, &AC_PlayerController::UseFirstSkill);
	InputComponent->BindAction("UseSecondSkill", IE_Pressed, this, &AC_PlayerController::UseSecondSkill);
}

void AC_PlayerController::SpinCharacterToMouse()
{
	if (m_MainCharacter)
	{
		//m_MouseHitResult;

		if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, m_MouseHitResult))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, FString::Printf(TEXT("Impact Point: %s"), *m_MouseHitResult.ImpactPoint.ToCompactString()));
			m_CursorPosition = m_MouseHitResult.ImpactPoint;
			
			m_CursorRotation = UKismetMathLibrary::FindLookAtRotation(m_MainCharacter->GetActorLocation(), m_CursorPosition);

			// 			m_MainCharacter->m_CursorPosition = m_CursorPosition;
			// 			m_MainCharacter->m_CursorRotation = m_CursorRotation;

			m_MainCharacter->SetActorRotation(FRotator(0.f, m_CursorRotation.Yaw, 0.f));
		}
	}
}

void AC_PlayerController::EvaluateMovementInput(float deltaTime)
{
	if (m_MainCharacter)
	{
		// Find movement direction
		const float upValue = GetInputAxisValue(m_MoveUpBinding);
		const float rightValue = GetInputAxisValue(m_MoveRightBinding);

		// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
		const FVector moveDirection = FVector(upValue, rightValue, 0.f).GetClampedToMaxSize(1.0f);
		m_MainCharacter->InputMovement(moveDirection);
	}
}

void AC_PlayerController::PressLeftClick()
{
	switch (m_InputMode)
	{
	case E_InputMode::INPUT_CHAR:
		ToggleMainAttack(true);
		break;
	}
}

void AC_PlayerController::ReleaseLeftClick()
{
	switch (m_InputMode)
	{
	case E_InputMode::INPUT_CHAR:
		ToggleMainAttack(false);
		break;
	}
}

void AC_PlayerController::PressRightClick()
{
	switch (m_InputMode)
	{
	case E_InputMode::INPUT_CHAR:
		ToggleSecondAttack(true);
		break;
	}
}

void AC_PlayerController::ReleaseRightClick()
{

}

void AC_PlayerController::PressLeftShoulder()
{

}

void AC_PlayerController::ReleaseLeftShoulder()
{

}

void AC_PlayerController::PressRightShoulder()
{

}

void AC_PlayerController::ReleaseRightShoulder()
{

}


void AC_PlayerController::ToggleMainAttack(bool isStarting)
{
	if (m_MainCharacter)
	{
		if (isStarting)
		{
			SpinCharacterToMouse();
			m_MovementMode = E_MovementMode::AIMING_MODE;
			m_MainCharacter->StartWeapon();
		}
		else
		{
			m_MovementMode = E_MovementMode::MOVEMENT_MODE;
			m_MainCharacter->StopWeapon();
		}
	}
}

void AC_PlayerController::ToggleSecondAttack(bool isStarting)
{
	if (m_MainCharacter)
	{
		if (isStarting)
		{
			//m_MovementMode = E_MovementMode::AIMING_MODE;
			m_MainCharacter->MeleeAttack();
		}
		else
		{
			//m_MovementMode = E_MovementMode::MOVEMENT_MODE;

		}
	}
}

void AC_PlayerController::UseFirstSkill()
{
	switch (m_InputMode)
	{
	case E_InputMode::INPUT_CHAR:
		if (m_MainCharacter)
		{
			//m_MainCharacter->UseFirstSkill();
		}
		break;
	}
}

void AC_PlayerController::UseSecondSkill()
{
	switch (m_InputMode)
	{
	case E_InputMode::INPUT_CHAR:
		if (m_MainCharacter)
		{
			//m_MainCharacter->UseSecondSkill();
		}
		break;
	}
}

void AC_PlayerController::ReloadWeapon()
{
	switch (m_InputMode)
	{
	case E_InputMode::INPUT_CHAR:
		if (m_MainCharacter)
		{
			m_MainCharacter->ReloadWeapon();
		}
		break;
	}
}

void AC_PlayerController::SwapWeapon()
{
	switch (m_InputMode)
	{
	case E_InputMode::INPUT_CHAR:
		if (m_MainCharacter)
		{
			m_MainCharacter->SwapWeapon();
		}
		break;
	}

}

void AC_PlayerController::ConfirmAction()
{
	switch (m_InputMode)
	{
	case E_InputMode::INPUT_CHAR:
		if (m_MainCharacter)
		{
			m_MainCharacter->Interact();
		}
		break;
	}

}

void AC_PlayerController::CancelAction()
{
	switch (m_InputMode)
	{
	case E_InputMode::INPUT_CHAR:
		if (m_MainCharacter)
		{
			m_MainCharacter->Dodge();
		}
		break;
	}
}

void AC_PlayerController::EscapeAction()
{
	switch (m_InputMode)
	{
	case E_InputMode::INPUT_CHAR:

		break;

	case E_InputMode::INPUT_MENU:

		break;
	}
}

#pragma endregion