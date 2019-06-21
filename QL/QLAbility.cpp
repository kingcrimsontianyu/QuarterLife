//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLAbility.h"
#include "Components/SphereComponent.h"
#include "QLCharacter.h"
#include "QLPlayerController.h"
#include "QLAbilityManager.h"
#include "Kismet/GameplayStatics.h"
#include "QLUtility.h"
#include "TimerManager.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLAbility::AQLAbility()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    QLName = "None";

    RootSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AQLAbility::OnComponentBeginOverlapImpl);

    DamageMultiplier = 1.0;
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLAbility::BeginPlay()
{
	Super::BeginPlay();

}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbility::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbility::OnUse()
{

}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbility::SetAbilityManager(UQLAbilityManager* AbilityManagerExt)
{
    AbilityManager = AbilityManagerExt;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbility::SetDamageMultiplier(const float Value)
{
    DamageMultiplier = Value;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbility::OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AQLCharacter* QLCharacter = Cast<AQLCharacter>(OtherActor);
    if (QLCharacter)
    {
        QLCharacter->AddAbility(this);
        QLCharacter->SetCurrentAbility(this->GetQLName());
        PlaySound("PickUp");
    }
}
