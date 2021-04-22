// Lin & Yin 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_PickUp.generated.h"

class UStaticMeshComponent;
class UBoxComponent;


UCLASS()
class RUNGUN_API AC_PickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	AC_PickUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* m_BoxComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Model, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_StaticMesh;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
