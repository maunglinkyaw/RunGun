#include "GM_Base.h"
#include "Characters/C_PlayerController.h"
#include "Skills/C_SkillManager.h"

AGM_Base::AGM_Base()
{
	PlayerControllerClass = AC_PlayerController::StaticClass();
	//PlayerStateClass = AC_PlayerState::StaticClass();

	m_SkillManager = CreateDefaultSubobject<UC_SkillManager>(TEXT("m_SkillManager"));

}
