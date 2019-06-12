//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLWeaponRailGun.h"
#include "QLCharacter.h"
#include "QLUtility.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/DamageType.h"
#include "Components/AudioComponent.h"
#include "QLWeaponManager.h"
#include "QLUmgUserWidget.h"
#include "QLPlayerController.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponRailGun::AQLWeaponRailGun()
{
    WeaponName = FName("RailGun");
    HitRange = 1e5f;
    RateOfFire = 1.5f;

    BasicDamage = 80.0f;
    ScopeDamage = 90.0f;

    RailBeamClass = AQLRailBeam::StaticClass();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRailGun::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRailGun::OnFire()
{
    PlayFireAnimation(FName("Fire"));

    PlayFireSound(FName("RailGunShot"));

    // create the transient beam actor
    UParticleSystemComponent* BeamComponentTemp = nullptr;
    AQLRailBeam* RailBeamTemp = nullptr;

    if (RailBeamClass)
    {
        RailBeamTemp = GetWorld()->SpawnActor<AQLRailBeam>(RailBeamClass, GetMuzzleLocation(), FRotator::ZeroRotator);
        if (RailBeamTemp)
        {
            BeamComponentTemp = RailBeamTemp->GetBeamComponent();
            if (BeamComponentTemp)
            {
                BeamComponentTemp->SetBeamSourcePoint(0, GetMuzzleLocation(), 0);
            }
        }
    }

    // ray tracing
    AQLCharacter* User = GetWeaponManager()->GetUser();

    if (User == nullptr)
    {
        return;
    }

    FHitResult HitResult = User->RayTraceFromCharacterPOV(HitRange);

    // if hit does not occur
    if (!HitResult.bBlockingHit)
    {
        FVector TargetLocation = GetMuzzleLocation() + User->GetFirstPersonCameraComponent()->GetForwardVector() * HitRange;

        if (BeamComponentTemp)
        {
            BeamComponentTemp->SetBeamTargetPoint(0, TargetLocation, 0);
        }

        return;
    }
    else
    {
        BeamComponentTemp->SetBeamTargetPoint(0, HitResult.ImpactPoint, 0);
    }

    // check the hit actor
    auto* hitActor = Cast<AQLCharacter>(HitResult.GetActor());
    if (!hitActor)
    {
        // do sth
        return;
    }

    // if self-hurt
    if (hitActor == User)
    {
        // do sth
        return;
    }

    // create a damage event
    TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
    FDamageEvent DamageEvent(ValidDamageTypeClass);

    const float DamageAmount = BasicDamage;
    hitActor->TakeDamage(DamageAmount, DamageEvent, User->GetController(), this);

    // display damage
    AQLPlayerController* QLPlayerController = User->GetQLPlayerController();
    if (!QLPlayerController)
    {
        return;
    }

    UQLUmgUserWidget* UMG = QLPlayerController->GetUMG();
    if (!UMG)
    {
        return;
    }

    int32 DamageAmountInt = FMath::RoundToInt(DamageAmount);
    UMG->ShowDamageOnScreen(FString::FromInt(DamageAmountInt), HitResult.ImpactPoint);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRailGun::OnAltFire()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRailGun::OnAltFireRelease()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRailGun::OnAltFireHold()
{
}
