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
#include "QLWeaponManager.h"
#include "QLCharacter.h"
#include "QLUtility.h"
#include "Engine/World.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponRocketLauncher::AQLWeaponRocketLauncher()
{
    WeaponName = FName("RocketLauncher");
    RateOfFire = 0.8f;

    BasicDamage = 100.0f;
    bFireEnabled = true;

    RocketProjectileClass = AQLRocketProjectile::StaticClass();
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

    // spawn a rocket
    UWorld* const World = GetWorld();
    GetMuzzleLocation();
    if (RocketProjectileClass && World && WeaponManager)
    {
        AQLCharacter* Character = WeaponManager->GetUser();

        if (!Character)
        {
            return;
        }

        const FRotator SpawnRotation = Character->GetControlRotation();
        const FVector SpawnLocation = GetMuzzleLocation();

        FActorSpawnParameters ActorSpawnParams;
        World->SpawnActor<AQLRocketProjectile>(RocketProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
    }

    PlayFireAnimation(FName("Fire"));

    PlayFireSoundFireAndForget(FName("Fire"));
}
