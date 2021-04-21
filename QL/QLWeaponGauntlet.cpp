//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLWeaponGauntlet.h"
#include "Components/AudioComponent.h"
#include "QLUtility.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponGauntlet::AQLWeaponGauntlet()
{
    SoundComponentEngineStart = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundComponentEngineStart"));
    SoundComponentEngineStart->SetupAttachment(RootComponent);
    SoundComponentEngineStart->bAutoActivate = false;

    SoundComponentConstRot = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundComponentConstRot"));
    SoundComponentConstRot->SetupAttachment(RootComponent);
    SoundComponentConstRot->bAutoActivate = false;

    SoundComponentEngineStop = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundComponentEngineStop"));
    SoundComponentEngineStop->SetupAttachment(RootComponent);
    SoundComponentEngineStop->bAutoActivate = false;

    QLName = FName(TEXT("Gauntlet"));
    //HitRange = 1200.0f;
    RateOfFire = 0.025f;
    bIsFireHeld = false;

    BasicDamage = 75.0f;
    //KnockbackSpeedChange = 50.0f;

    FireMontage = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponGauntlet::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    SetDamageMultiplier(1.0f);

    FireMontage = GetAnimationMontage(FName(TEXT("Fire")));
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponGauntlet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponGauntlet::OnFire()
{
    // if we are still in the fire disabled window, the weapon cannot be used
    if (!bFireEnabled)
    {
        return;
    }

    // enforce rate of fire
    bFireEnabled = false;
    GetWorldTimerManager().SetTimer(DisableFireTimerHandle,
        this,
        &AQLWeaponGauntlet::EnableFireCallBack,
        1.0f, // time interval in second. since loop is not used,
              // this parameter can be an arbitrary value except 0.0f.
        false, // loop
        RateOfFire); // delay in second

    // allow continuous shooting
    if (bIsFireHeld)
    {
        return;
    }

    bIsFireHeld = true;

    GetWorldTimerManager().SetTimer(HoldFireTimerHandle,
        this,
        &AQLWeaponGauntlet::PerformDrill,
        RateOfFire, // time interval in second
        true, // loop
        0.0f); // delay in second
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponGauntlet::OnFireRelease()
{
    if (!bIsFireHeld)
    {
        return;
    }

    bIsFireHeld = false;

    GetWorldTimerManager().ClearTimer(HoldFireTimerHandle);

    if (AnimInstanceWeapon.IsValid() && FireMontage)
    {
        AnimInstanceWeapon->Montage_JumpToSection(FName(TEXT("SecEngineStop")), FireMontage);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponGauntlet::PerformDrill()
{
    if (AnimInstanceWeapon.IsValid() && FireMontage)
    {
        // prevent the animation from repeatedly returning to the start when fire button is held
        if (!AnimInstanceWeapon->Montage_IsPlaying(FireMontage))
        {
            // Note: If the gauntlet is fired at high rate by rapidly left clicking the mouse,
            // it will appear as if the section SecEngineStart is repeatedly played. That is not true.
            // It is actually the section SecEngineStop that is being repeatedly played.
            AnimInstanceWeapon->Montage_Play(FireMontage);
            AnimInstanceWeapon->Montage_JumpToSection(FName(TEXT("SecEngineStart")), FireMontage);
        }
    }
}