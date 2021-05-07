// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "C_GameplayHUD.generated.h"

class UCUI_GameplayScreen;

UCLASS()
class RUNGUN_API AC_GameplayHUD : public AHUD
{
	GENERATED_BODY()
public:
	AC_GameplayHUD();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UCUI_GameplayScreen> m_GameplayUIClass = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = UI)
	UCUI_GameplayScreen* m_GameplayUI = nullptr;



	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void DrawHUD() override;

};
