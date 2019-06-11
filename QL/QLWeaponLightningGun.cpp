//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------

#include "QLWeaponLightningGun.h"
#include "QLCharacter.h"
#include "QLUtility.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/DamageType.h"
#include "Components/AudioComponent.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponLightningGun::AQLWeaponLightningGun()
{
    WeaponName = FName("LightningGun");
    HitRange = 1200.0f;
    RateOfFire = 0.05f;
    bIsFireHeld = false;

    BasicDamage = 6.0f;
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
        OnFireHold();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponLightningGun::OnFire()
{
    PlayFireSound(FName("ElectricZap"));

    bIsFireHeld = true;

    // handle beam
    if (BeamComponent)
    {
        BeamComponent->Activate();
    }

    GetWorldTimerManager().SetTimer(HeldDownFireTimerHandle,
                                    this,
                                    &AQLWeaponLightningGun::InflictDamage,
                                    RateOfFire, // time interval in second
                                    true, // loop
                                    0.0f); // delay in second
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponLightningGun::OnFireRelease()
{
    StopFireSound();

    bIsFireHeld = false;

    if (BeamComponent)
    {
        BeamComponent->Deactivate();
    }

    GetWorldTimerManager().ClearTimer(HeldDownFireTimerHandle);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponLightningGun::OnFireHold()
{
    // to do: in order to ensure correctness, for each tick, ray trace is performed twice, one in Tick(), the other in HasHitEnemy()
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

    // repeat fire sound
    if (!FireSoundComponent->IsPlaying())
    {
        FireSoundComponent->Play();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponLightningGun::InflictDamage()
{
    if (User == nullptr)
    {
        return;
    }

    FHitResult HitResult = User->RayTraceFromCharacterPOV(HitRange);

    // if hit does not occur
    if (!HitResult.bBlockingHit)
    {
        // do sth
        QLUtility::Log("AQLWeaponLightningGun: no hit");
        return;
    }

    // if hit occurs, check the hit actor
    auto* hitActor = Cast<AQLCharacter>(HitResult.GetActor());
    if (!hitActor)
    {
        // do sth
        QLUtility::Log("AQLWeaponLightningGun: does not hit AQLCharacter");
        return;
    }

    // if self-hurt
    if (hitActor == User)
    {
        // do sth
        QLUtility::Log("AQLWeaponLightningGun: only hit player himself");
        return;
    }

    // create a damage event
    TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
    FDamageEvent DamageEvent(ValidDamageTypeClass);

    const float DamageAmount = BasicDamage;
    hitActor->TakeDamage(DamageAmount, DamageEvent, User->GetController(), this);
}