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
#include "Camera/CameraComponent.h"
#include "QLRocketProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

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

    PlayFireAnimation(FName("Fire"));

    PlayFireSoundFireAndForget(FName("Fire"));

    // ray tracing
    AQLCharacter* User = GetWeaponManager()->GetUser();

    if (User == nullptr)
    {
        return;
    }

    FHitResult HitResult = User->RayTraceFromCharacterPOV(HitRange);

    // determine source and target
    UCameraComponent* CameraComponent = User->GetFirstPersonCameraComponent();
    if (CameraComponent == nullptr)
    {
        return;
    }

    FVector SourceLocation = GetMuzzleLocation() + CameraComponent->GetForwardVector() * 10.0f;
    FVector TargetLocation;

    // if hit occurs
    if (HitResult.bBlockingHit)
    {
        TargetLocation = HitResult.ImpactPoint;
    }
    else
    {
        TargetLocation = GetMuzzleLocation() + CameraComponent->GetForwardVector() * HitRange;
    }

    FVector ProjectileForwardVector = TargetLocation - SourceLocation;
    ProjectileForwardVector.Normalize();

    FMatrix result = FRotationMatrix::MakeFromXZ(ProjectileForwardVector, CameraComponent->GetUpVector());
    FRotator SourceRotation = result.Rotator();

    // spawn and launch a rocket
    UWorld* const World = GetWorld();
    GetMuzzleLocation();
    if (RocketProjectileClass && World && WeaponManager)
    {
        if (!User || !CameraComponent)
        {
            return;
        }

        FActorSpawnParameters ActorSpawnParams;
        AQLRocketProjectile* Rocket = World->SpawnActor<AQLRocketProjectile>(RocketProjectileClass, SourceLocation, SourceRotation, ActorSpawnParams);
        if (Rocket)
        {
            // change velocity

            FVector FinalVelocity = ProjectileForwardVector * Rocket->GetProjectileMovementComponent()->InitialSpeed;
            Rocket->GetProjectileMovementComponent()->Velocity = FinalVelocity;
        }
    }
}