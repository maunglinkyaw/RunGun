// Lin & Yin 2020


#include "C_PickUp.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "RunGun/Characters/C_MainCharacter.h"
#include "RunGun/Skills/Weapons/C_Weapon.h"

AC_PickUp::AC_PickUp()
{
	PrimaryActorTick.bCanEverTick = false;

	m_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = m_BoxComponent;
	m_BoxComponent->SetSimulatePhysics(true);
	m_BoxComponent->SetCollisionProfileName(FName("Ragdoll"));
	m_BoxComponent->BodyInstance.bLockXRotation = true;
	m_BoxComponent->BodyInstance.bLockYRotation = true;

	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	m_StaticMesh->SetupAttachment(RootComponent);
	m_StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AC_PickUp::BeginPlay()
{
	Super::BeginPlay();
	
	m_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AC_PickUp::OnBeginOverlap);
	m_BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AC_PickUp::OnEndOverlap);

}

// Called every frame
void AC_PickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_PickUp::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AC_MainCharacter* mainChar = Cast<AC_MainCharacter>(OtherActor))
	{
		mainChar->RegisterPickUp(this);
	}
}

void AC_PickUp::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AC_MainCharacter* mainChar = Cast<AC_MainCharacter>(OtherActor))
	{
		mainChar->UnregisterPickUp(this);
	}
}