//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLWeaponRocketLauncher.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponRocketLauncher::AQLWeaponRocketLauncher()
{
    WeaponName = FName("RocketLauncher");
    RateOfFire = 0.8f;

    BasicDamage = 100.0f;
    bFireEnabled = true;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRocketLauncher::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRocketLauncher::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRocketLauncher::OnFire()
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
                                    &AQLWeaponRocketLauncher::EnableFireCallBack,
                                    1.0f, // time interval in second. since loop is not used,
                                          // this parameter can be an arbitrary value except 0.0f.
                                    false, // loop
                                    RateOfFire); // delay in second

    PlayFireAnimation(FName("Fire"));

    PlayFireSoundFireAndForget(FName("Fire"));
}
