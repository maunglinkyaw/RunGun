// Lin & Yin 2020

#include "C_GameplayHUD.h"
#include "CUI_GameplayScreen.h"

#include "RunGun/RunGun.h"

AC_GameplayHUD::AC_GameplayHUD()
{

}

void AC_GameplayHUD::BeginPlay()
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

void AC_GameplayHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AC_GameplayHUD::DrawHUD()
{
	Super::DrawHUD();

}
