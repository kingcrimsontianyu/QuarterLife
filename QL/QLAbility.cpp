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
#include "QLCharacter.h"
#include "QLUmgFirstPerson.h"

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
    bCanBeUsed = true;
    CooldownDuration = 10.0f;
    CooldownTimeElapsed = 0.0f;
    CooldownUpdateTimeIncrement = 0.1f;
    CooldownPercent = 0.0f;
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
    if (!IsActive())
    {
        return;
    }

    CooldownTimeElapsed = 0.0f;

    // until the next reactivate
    GetWorldTimerManager().SetTimer(CooldownDurationTimerHandle,
        this,
        &AQLAbility::Reactivate,
        1.0f, // time interval in second
        false, // loop
        CooldownDuration); // delay in second

    // once the effect starts, periodically update
    GetWorldTimerManager().SetTimer(CooldownTimeElapsedTimerHandle,
        this,
        &AQLAbility::UpdateProgressOnUMG,
        CooldownUpdateTimeIncrement, // time interval in second
        true, // loop
        0.0f); // delay in second
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

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbility::UpdateProgressOnUMGInternal(const float Value)
{
    // update the currently used ability's cooldown progress on umg
    if (AbilityManager.IsValid())
    {
        if (AbilityManager->GetCurrentAbility() != this)
        {
            return;
        }

        auto* Character = AbilityManager->GetUser();

        if (!Character)
        {
            return;
        }

        auto* PlayerController = Character->GetQLPlayerController();
        if (PlayerController)
        {
            auto* UMG = PlayerController->GetUMG();
            if (UMG)
            {
                UMG->UpdateAbilityCooldownProgress(Value);
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbility::UpdateProgressOnUMG()
{
    CooldownTimeElapsed += CooldownUpdateTimeIncrement;
    CooldownPercent = CooldownTimeElapsed / CooldownDuration;

    CooldownPercent = CooldownPercent < 0.0f ? 0.0f : CooldownPercent;
    CooldownPercent = CooldownPercent > 1.0f ? 1.0f : CooldownPercent;

    UpdateProgressOnUMGInternal(CooldownPercent);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbility::Reactivate()
{
    bCanBeUsed = true;

    // reset
    UpdateProgressOnUMGInternal(0.0f);

    GetWorldTimerManager().ClearTimer(CooldownDurationTimerHandle);
    GetWorldTimerManager().ClearTimer(CooldownTimeElapsedTimerHandle);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbility::Deactivate()
{
    bCanBeUsed = false;
}

//------------------------------------------------------------
//------------------------------------------------------------
bool AQLAbility::IsActive()
{
    return bCanBeUsed;
}