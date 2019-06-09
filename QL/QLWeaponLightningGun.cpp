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
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraComponent.h"

//----------------------------------------
//----------------------------------------
AQLWeaponLightningGun::AQLWeaponLightningGun()
{
    WeaponName = FName("LightningGun");
    HitRange = 2000.0f;
    RateOfFire = 0.048f;
    bIsFireHeld = false;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponLightningGun::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (BeamComponent)
    {
        // BeamComponent->bAutoActivate = false does not work
        BeamComponent->Deactivate();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponLightningGun::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsFireHeld)
    {
        // to do: ray trace is performed twice, one in Tick(), the other in HasHitEnemy(), to ensure correctness
        // need to understand the tick order and simplify the calculation
        FHitResult HitResult = User->RayTraceFromCharacterPOV(HitRange);

        BeamComponent->SetBeamSourcePoint(0, GetMuzzleLocation(), 0);

        // if hit does not occur
        if (!HitResult.bBlockingHit)
        {
            FVector TargetLocation = GetMuzzleLocation() + User->GetFirstPersonCameraComponent()->GetForwardVector() * HitRange;
            BeamComponent->SetBeamTargetPoint(0, TargetLocation, 0);
        }
        else
        {
            // if hit occurs, handle beam
            if (BeamComponent)
            {
                BeamComponent->SetBeamTargetPoint(0, HitResult.ImpactPoint, 0);
            }
        }
    }
}

//----------------------------------------
//----------------------------------------
void AQLWeaponLightningGun::Fire()
{
    bIsFireHeld = true;

    // handle beam
    if (BeamComponent)
    {
        BeamComponent->Activate();
    }

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
    bIsFireHeld = false;

    if (BeamComponent)
    {
        BeamComponent->Deactivate();
    }

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