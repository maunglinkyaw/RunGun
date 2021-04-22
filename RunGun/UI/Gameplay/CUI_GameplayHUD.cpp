// Lin & Yin 2020

#include "CUI_GameplayHUD.h"
#include "CUI_GameplayScreen.h"

#include "RunGun/RunGun.h"

ACUI_GameplayHUD::ACUI_GameplayHUD()
{

}

void ACUI_GameplayHUD::BeginPlay()
{
	Super::BeginPlay();

	if (m_GameplayUIClass)
	{
		m_GameplayUI = CreateWidget<UCUI_GameplayScreen>(GetWorld(), m_GameplayUIClass);

		if (m_GameplayUI)
		{
			m_GameplayUI->AddToViewport();
		}
	}

	UE_LOG(LogUI, Log, TEXT("%s - BeginPlay - m_GameplayUI: %s"), *GetName(), *GetNameSafe(m_GameplayUI));
}

void ACUI_GameplayHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ACUI_GameplayHUD::DrawHUD()
{
	Super::DrawHUD();

}
