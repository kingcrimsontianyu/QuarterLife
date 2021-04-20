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
#include "QLUtility.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponGauntlet::AQLWeaponGauntlet()
{
    QLName = FName(TEXT("Gauntlet"));
    //HitRange = 1200.0f;
    RateOfFire = 0.025f;
    bIsFireHeld = false;

    BasicDamage = 75.0f;
    //KnockbackSpeedChange = 50.0f;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponGauntlet::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    SetDamageMultiplier(1.0f);
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

    PlayWeaponAnimationMontageJumpToSectionsEnd(FName(TEXT("Fire")), FName(TEXT("SecConstRot")));
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponGauntlet::PerformDrill()
{
    PlayWeaponAnimationMontage(FName(TEXT("Fire")));
}