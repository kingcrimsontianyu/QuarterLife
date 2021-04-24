//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLWeaponNailGun.h"
#include "QLWeaponManager.h"
#include "QLCharacter.h"
#include "QLUtility.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "QLNailProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "QLPlayerController.h"
#include "Kismet/GameplayStatics.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponNailGun::AQLWeaponNailGun()
{
    QLName = FName(TEXT("NailGun"));
    WeaponType = EQLWeapon::NailGun;

    RateOfFire = 0.1f;
    NailProjectileClass = AQLNailProjectile::StaticClass();

    bIsFireHeld = false;
    bIsProjectileWeapon = true;
    ProjectileSpeed = 1500.0f;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponNailGun::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    SetDamageMultiplier(1.0f);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponNailGun::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponNailGun::OnFire()
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
        &AQLWeaponNailGun::EnableFireCallBack,
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
        &AQLWeaponNailGun::SpawnProjectile,
        RateOfFire, // time interval in second
        true, // loop
        0.0f); // delay in second
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponNailGun::OnFireRelease()
{
    if (!bIsFireHeld)
    {
        return;
    }

    bIsFireHeld = false;

    GetWorldTimerManager().ClearTimer(HoldFireTimerHandle);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponNailGun::SpawnProjectile()
{
    PlaySoundFireAndForget(FName(TEXT("Fire")));

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

    // spawn and launch a Nail
    UWorld* const World = GetWorld();

    if (NailProjectileClass && World && WeaponManager.IsValid())
    {
        if (!User || !CameraComponent)
        {
            return;
        }

        FTransform MyTransform(SourceRotation, SourceLocation, FVector(1.0f));
        AQLNailProjectile* Nail = World->SpawnActorDeferred<AQLNailProjectile>(NailProjectileClass, MyTransform);

        // pass controller to Nail as damage instigator
        AController* Controller = User->GetController();
        AQLPlayerController* QLPlayerController = Cast<AQLPlayerController>(Controller);
        Nail->QLSetPlayerController(QLPlayerController);
        Nail->SetDamageMultiplier(DamageMultiplier);
        UGameplayStatics::FinishSpawningActor(Nail, MyTransform);

        // change velocity
        Nail->GetProjectileMovementComponent()->MaxSpeed = ProjectileSpeed;
        Nail->GetProjectileMovementComponent()->InitialSpeed = ProjectileSpeed;
        FVector FinalVelocity = ProjectileForwardVector * ProjectileSpeed;
        Nail->GetProjectileMovementComponent()->Velocity = FinalVelocity;
    }
}