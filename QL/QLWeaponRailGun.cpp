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
    ZoomDamage = 90.0f;
    CurrentDamage = BasicDamage;

    bZoomedIn = false;
    FOVCached = 90.0f;
    CameraComponentCached = nullptr;
    bFireEnabled = true;

    RailBeamClass = AQLRailBeam::StaticClass();

    // animation
    ZoomTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ZoomTimeline"));
    ZoomTimelineInterpFunction.BindUFunction(this, FName{ TEXT("ZoomCallback") });
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRailGun::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (ZoomTimeline && ZoomCurve)
    {
        ZoomTimeline->AddInterpFloat(ZoomCurve, ZoomTimelineInterpFunction, FName(TEXT("Zoom")));
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRailGun::OnFire()
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
        &AQLWeaponRailGun::EnableFire,
        1.0f, // time interval in second. since loop is not used,
              // this parameter can be an arbitrary value except 0.0f.
        false, // loop
        RateOfFire); // delay in second

    PlayFireAnimation(FName("Fire"));

    PlayFireSoundFireAndForget(FName("RailGunShot"));

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

    hitActor->TakeDamage(CurrentDamage, DamageEvent, User->GetController(), this);

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

    int32 DamageAmountInt = FMath::RoundToInt(CurrentDamage);
    UMG->ShowDamageOnScreen(FString::FromInt(DamageAmountInt), HitResult.ImpactPoint);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRailGun::OnAltFire()
{
    bZoomedIn = true;
    CurrentDamage = ZoomDamage;

    // animation
    if (ZoomTimeline && ZoomCurve)
    {
        if (ZoomTimeline->IsPlaying())
        {
            ZoomTimeline->Stop();
        }
        // if the zoom animation is not currently played
        // get the original fov and cache it
        else
        {
            // fov
            if (WeaponManager)
            {
                AQLCharacter* QLCharacter = WeaponManager->GetUser();
                if (QLCharacter)
                {
                    CameraComponentCached = QLCharacter->GetFirstPersonCameraComponent();
                    if (CameraComponentCached)
                    {
                        FOVCached = CameraComponentCached->FieldOfView;
                    }
                }
            }
        }

        ZoomTimeline->Play();
    }

    // sound
    PlayFireSound(FName("RailGunCharge"));
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRailGun::OnAltFireRelease()
{
    bZoomedIn = false;
    CurrentDamage = BasicDamage;

    // animation
    if (ZoomTimeline && ZoomCurve)
    {
        if (ZoomTimeline->IsPlaying())
        {
            ZoomTimeline->Stop();
        }

        ZoomTimeline->Reverse();
    }

    // sound
    StopFireSound();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRailGun::OnAltFireHold()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRailGun::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRailGun::ZoomCallback(float Val)
{
    if (CameraComponentCached)
    {
        CameraComponentCached->SetFieldOfView(FOVCached + Val);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRailGun::PrepareForImpendingWeaponSwitch()
{
    // reset fov
    if (ZoomTimeline->IsPlaying())
    {
        ZoomTimeline->Stop();
    }

    if (CameraComponentCached)
    {
        CameraComponentCached->SetFieldOfView(FOVCached);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRailGun::EnableFire()
{
    bFireEnabled = true;

    GetWorldTimerManager().ClearTimer(DisableFireTimerHandle);
}
