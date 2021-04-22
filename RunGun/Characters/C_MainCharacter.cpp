// Lin & Yin 2020

#include "C_MainCharacter.h"
#include "C_PlayerController.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "RunGun/Skills/Weapons/C_Weapon.h"
#include "RunGun/Items/C_PickUp.h"

#include "RunGun/RunGun.h"

AC_MainCharacter::AC_MainCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(20.f, 60.0f);
	GetCapsuleComponent()->SetCollisionProfileName("PlayerPawn");

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create the box that will determine the interact range.
	m_InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	m_InteractBox->SetupAttachment(RootComponent);
	m_InteractBox->SetSimulatePhysics(false);
	m_InteractBox->SetBoxExtent(FVector(50.f, 50.f, 60.f));
	m_InteractBox->SetRelativeLocationAndRotation(FVector(40.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));

	// Create a camera boom attached to the root (capsule)
	m_CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_CameraArm->SetupAttachment(RootComponent);
	m_CameraArm->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	m_CameraArm->bDoCollisionTest = false;
	m_CameraArm->TargetArmLength = 950.f;
	m_CameraArm->SocketOffset = FVector(0.f, 0.f, 0.f);
	m_CameraArm->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));
	//m_CameraArm->SetWorldRotation(FRotator(-45.f, 0.f, 0.f));

	// Create a camera and attach to boom
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopViewCamera"));
	m_Camera->SetupAttachment(m_CameraArm, USpringArmComponent::SocketName);
	m_Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera
	m_Camera->SetProjectionMode(ECameraProjectionMode::Perspective);
	m_Camera->SetFieldOfView(100.f);
	m_Camera->SetOrthoWidth(1920.f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Do not face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->GravityScale = 4.f;
	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->JumpZVelocity = 800.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

// 	static ConstructorHelpers::FObjectFinder<UClass> gunClass(TEXT("Blueprint'/Game/Weapons/CW_Hexer_BP.CW_Hexer_BP_C'"));
// 	m_TestWeaponClass = gunClass.Object;

// 	m_Weapons.Add(E_WeaponClass::MAIN_WEAPON, nullptr);
// 	m_Weapons.Add(E_WeaponClass::SIDE_WEAPON, nullptr);
}

void AC_MainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	m_InteractBox->OnComponentBeginOverlap.AddDynamic(this, &AC_MainCharacter::InteractBeginOverlap);
	m_InteractBox->OnComponentEndOverlap.AddDynamic(this, &AC_MainCharacter::InteractEndOverlap);

	UE_LOG(LogCharacters, Log, TEXT("%s - BeginPlay()"), *GetName());


	if (m_SideTestWeaponClass)
	{
		AC_Weapon* tempWeapon = GetWorld()->SpawnActor<AC_Weapon>(m_SideTestWeaponClass);

		if (tempWeapon)
			PickUpNewWeapon(tempWeapon);
	}
	if (m_TestWeaponClass)
	{
		AC_Weapon* tempWeapon = GetWorld()->SpawnActor<AC_Weapon>(m_TestWeaponClass);

		if (tempWeapon)
			PickUpNewWeapon(tempWeapon);
	}
}

void AC_MainCharacter::RegisterPlayerController(AC_PlayerController* inController)
{
	UE_LOG(LogCharacters, Log, TEXT("%s - RegisterPlayerController - inController: %s"), *GetName(), *GetNameSafe(inController));

	m_Controller = inController;
	m_GameplayHUD = m_Controller->m_GameplayHUD;

}


void AC_MainCharacter::InteractBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogCharacters, Log, TEXT("%s - InteractBeginOverlap - otherActor: %s"), *GetName(), *GetNameSafe(OtherActor));

}

void AC_MainCharacter::InteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogCharacters, Log, TEXT("%s - InteractEndOverlap - otherActor: %s"), *GetName(), *GetNameSafe(OtherActor));

}

void AC_MainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_IsInputBlocked)
	{
		EvaluateInputBlock(DeltaTime);
	}
}


void AC_MainCharacter::InputMovement(FVector moveDirection)
{
	m_DesiredMovement = moveDirection;

	if (moveDirection.SizeSquared() > 0.0f)
	{
		GetCharacterMovement()->AddInputVector(moveDirection);
	}

	// 	const FRotator NewRotation = moveDirection.Rotation();
	// 	FHitResult Hit(1.f);
	// 	RootComponent->MoveComponent(moveDirection, NewRotation, true, &Hit);
	// 
	// 	if (Hit.IsValidBlockingHit())
	// 	{
	// 		const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
	// 		const FVector Deflection = FVector::VectorPlaneProject(moveDirection, Normal2D) * (1.f - Hit.Time);
	// 		RootComponent->MoveComponent(Deflection, NewRotation, true);
	// 	}
}

void AC_MainCharacter::BlockPlayerInput(float duration)
{
// 	UE_LOG(LogCharacters, Log, TEXT("%s - BlockPlayerInput: %f"), *GetName(), duration);

	if (!m_IsInputBlocked)
	{
		m_IsInputBlocked = true;
		// Call UI to show here.
	}

	if (m_ActiveWeapon)
		m_ActiveWeapon->InterruptWeaponFunctions();

	m_InputBlockDuration = duration;
	m_InputBlockTimer = 0.f;
}

void AC_MainCharacter::EvaluateInputBlock(float deltaTime)
{
	m_InputBlockTimer += deltaTime;
	
	if (m_InputBlockTimer >= m_InputBlockDuration)
	{
		UnblockPlayerInput();
	}
}

void AC_MainCharacter::UnblockPlayerInput()
{
// 	UE_LOG(LogCharacters, Log, TEXT("%s - UnblockPlayerInput - timer: %f, duration: %f"), *GetName(), m_InputBlockTimer, m_InputBlockDuration);

	m_IsInputBlocked = false;
	m_InputBlockTimer = 0.f;

	if (m_IsPressingFire && m_ActiveWeapon && !m_ActiveWeapon->m_IsPressingFire)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString::Printf(TEXT("UnblockStartWeapon: %s"), *GetNameSafe(m_ActiveWeapon)));
		m_ActiveWeapon->StartWeapon();
	}
}

void AC_MainCharacter::ToggleMovement(bool canMove)
{ 
	if (canMove)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	else
	{
		GetCharacterMovement()->DisableMovement();
	}
}

bool AC_MainCharacter::CanDoAction()
{
	return !m_IsInputBlocked;

	// To Do, maybe add events for stunned etc.
}

void AC_MainCharacter::RegisterPickUp(AC_PickUp* newPickUp)
{

}

void AC_MainCharacter::UnregisterPickUp(AC_PickUp* removedPickUp)
{

}

void AC_MainCharacter::Dodge()
{
	if (m_IsInputBlocked)
		return;

	if (m_DesiredMovement.SizeSquared() > 0.0f)
	{
		LaunchCharacter(m_DesiredMovement * m_DodgeForce, true, false);
	}
	else
	{
		LaunchCharacter(GetActorForwardVector() * m_DodgeForce, true, false);
	}

	BlockPlayerInput(m_DodgeCooldown);
}

void AC_MainCharacter::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, FString::Printf(TEXT("%s - I'm Interacting!"), *GetName()));
	
	if (m_IsInputBlocked)
		return;

}

void AC_MainCharacter::StartWeapon()
{
	m_IsPressingFire = true;

	if (m_IsInputBlocked)
		return;

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, FString::Printf(TEXT("StartWeapon: %s"), *GetNameSafe(m_ActiveWeapon)));

	if (m_ActiveWeapon)
	{
		m_ActiveWeapon->StartWeapon();
	}

}

void AC_MainCharacter::StopWeapon()
{
	m_IsPressingFire = false;

	if (m_ActiveWeapon)
	{
		m_ActiveWeapon->StopWeapon();
	}
}

void AC_MainCharacter::ReloadWeapon()
{
	if (m_IsInputBlocked)
		return;

	if (m_ActiveWeapon)
	{
		m_ActiveWeapon->AttemptToReload();
	}

}

void AC_MainCharacter::StopReloadWeapon()
{
	if (m_ActiveWeapon)
	{
		m_ActiveWeapon->StopReloadWeapon();
	}
}

void AC_MainCharacter::SwapWeapon()
{
	if (m_IsInputBlocked)
		return;

	if (m_MainWeapon && m_SideWeapon)
	{
		if (m_ActiveWeapon != m_MainWeapon)
		{
			SwapToWeapon(m_MainWeapon);
		}
		else
		{
			SwapToWeapon(m_SideWeapon);
		}
	}
	else if (!m_ActiveWeapon)
	{
		if (m_MainWeapon)
		{
			SwapToWeapon(m_MainWeapon);
		}
		else if (m_SideWeapon)
		{
			SwapToWeapon(m_SideWeapon);
		}
	}
	else
	{
		UE_LOG(LogCharacters, Log, TEXT("%s - SwapWeapon - Failed - m_ActiveWeapon: %s, m_MainWeapon: %s, m_SideWeapon: %s"),
			*GetName(), *GetNameSafe(m_ActiveWeapon), *GetNameSafe(m_MainWeapon), *GetNameSafe(m_SideWeapon));
	}
}


void AC_MainCharacter::MeleeAttack()
{
	if (m_IsInputBlocked)
		return;

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, FString::Printf(TEXT("MeleeAttack")));

}

void AC_MainCharacter::UseFirstSkill()
{
	if (m_IsInputBlocked)
		return;


}

void AC_MainCharacter::UseSecondSkill()
{
	if (m_IsInputBlocked)
		return;


}


void AC_MainCharacter::PickUpNewWeapon(AC_Weapon* newWeapon)
{
	if (newWeapon)
	{
		if (newWeapon->m_IsMainWeapon)
		{
			if (m_MainWeapon)
				m_MainWeapon->DropWeapon();

			m_MainWeapon = newWeapon;
			newWeapon->GetPickedUpBy(this);
		}
		else
		{ 
			if (m_SideWeapon)
				m_SideWeapon->DropWeapon();

			m_SideWeapon = newWeapon;
			newWeapon->GetPickedUpBy(this);
		}

		SwapToWeapon(newWeapon);
	}
}


void AC_MainCharacter::SwapToWeapon(AC_Weapon* newWeapon)
{
	if (newWeapon)
	{
		if (m_ActiveWeapon)
			m_ActiveWeapon->HolsterWeapon();

		m_ActiveWeapon = newWeapon;
		m_ActiveWeapon->DrawWeapon();

		BlockPlayerInput(m_SwapCooldown);

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("%s - SwapToWeapon: %s"), *GetName(), *GetNameSafe(m_ActiveWeapon)));
	}
}



void AC_MainCharacter::AddAmmoToInventory(int32 weaponIndex, int32 ammoCount)
{
	switch (weaponIndex)
	{
	case 0:
		m_Inventory.mainAmmo += ammoCount;
		if (m_Inventory.mainAmmo > m_Inventory.maxMainAmmo)
			m_Inventory.mainAmmo = m_Inventory.maxMainAmmo;
		break;

	case 1:
		m_Inventory.sideAmmo += ammoCount;
		if (m_Inventory.sideAmmo > m_Inventory.maxSideAmmo)
			m_Inventory.sideAmmo = m_Inventory.maxSideAmmo;
		break;
	}

}

int32 AC_MainCharacter::RequestAmmoFromInventory(int32 weaponIndex, int32 ammoCount)
{
	UE_LOG(LogWeapons, Log, TEXT("%s - RequestAmmoFromInventory - requested: %d"), *GetName(), ammoCount);

	if (ammoCount < 0)
		return 0;

	switch (weaponIndex)
	{
	case 0:
		if (m_Inventory.mainAmmo < ammoCount)
		{
			ammoCount = m_Inventory.mainAmmo;
		}

		m_Inventory.mainAmmo -= ammoCount;
		break;

	case 1:
		if (m_Inventory.sideAmmo < ammoCount)
		{
			ammoCount = m_Inventory.sideAmmo;
		}

		m_Inventory.sideAmmo -= ammoCount;
		break;
	}
	
	UE_LOG(LogWeapons, Log, TEXT("%s - RequestAmmoFromInventory - returned: %d"), *GetName(), ammoCount);
	
	return ammoCount;
}

const int32 AC_MainCharacter::GetAmmoCount(int32 weaponIndex)
{
	switch (weaponIndex)
	{
	case 0:
		return m_Inventory.mainAmmo;
		break;

	case 1:
		return m_Inventory.sideAmmo;
		break;
	}

	return 0;
}
