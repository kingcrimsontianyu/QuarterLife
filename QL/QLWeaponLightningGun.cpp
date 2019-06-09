//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#include "QLWeaponLightningGun.h"
#include "QLCharacter.h"
#include "QLUtility.h"
#include "Engine/World.h"

//----------------------------------------
//----------------------------------------
AQLWeaponLightningGun::AQLWeaponLightningGun() :
HitRange(2000.0f),
RateOfFire(0.048f)
{

}

//----------------------------------------
//----------------------------------------
void AQLWeaponLightningGun::Fire()
{
    GetWorldTimerManager().SetTimer(HeldDownFireTimerHandle,
                                    this,
                                    &AQLWeaponLightningGun::DealDamageIfHit,
                                    RateOfFire, // time interval in second
                                    true, // loop
                                    0.0f); // delay in second
}

//----------------------------------------
//----------------------------------------
void AQLWeaponLightningGun::FireRelease()
{
    GetWorldTimerManager().ClearTimer(HeldDownFireTimerHandle);
}

//----------------------------------------
//----------------------------------------
bool AQLWeaponLightningGun::HasHitEnemy()
{
    if (User == nullptr)
    {
        return false;
    }

    FHitResult HitResult = User->RayTraceFromCharacterPOV(HitRange);

    // if hit does not occur
    if (!HitResult.bBlockingHit)
    {
        // do sth
        QLUtility::Log("AQLWeaponLightningGun: no hit");
        return false;
    }

    // if hit occurs, check the hit actor
    auto* hitActor = Cast<AQLCharacter>(HitResult.GetActor());
    if (!hitActor)
    {
        // do sth
        QLUtility::Log("AQLWeaponLightningGun: does not hit AQLCharacter");
        return false;
    }

    if (hitActor == User)
    {
        // do sth
        QLUtility::Log("AQLWeaponLightningGun: only hit player himself");
        return false;
    }

    return true;
}

//----------------------------------------
//----------------------------------------
void AQLWeaponLightningGun::DealDamageIfHit()
{
    if (HasHitEnemy())
    {
        QLUtility::Log("AQLWeaponLightningGun: deal dmg");
    }
}