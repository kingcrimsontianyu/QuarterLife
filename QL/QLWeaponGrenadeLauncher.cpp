//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLWeaponGrenadeLauncher.h"
#include "QLRecyclerGrenadeProjectile.h"
#include "QLWeaponManager.h"
#include "QLCharacter.h"
#include "QLUtility.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "QLPlayerController.h"
#include "Kismet/GameplayStatics.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponGrenadeLauncher::AQLWeaponGrenadeLauncher()
{
    QLName = FName(TEXT("GrenadeLauncher"));

    RateOfFire = 1.0f;

    RecyclerGrenadeProjectileClass = AQLRecyclerGrenadeProjectile::StaticClass();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponGrenadeLauncher::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponGrenadeLauncher::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponGrenadeLauncher::OnFire()
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
        &AQLWeaponGrenadeLauncher::EnableFireCallBack,
        1.0f, // time interval in second. since loop is not used,
              // this parameter can be an arbitrary value except 0.0f.
        false, // loop
        RateOfFire); // delay in second

    PlayAnimationMontage(FName(TEXT("Fire")));

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

    // spawn and launch a grenade
    UWorld* const World = GetWorld();

    if (RecyclerGrenadeProjectileClass && World && WeaponManager.IsValid())
    {
        if (!User || !CameraComponent)
        {
            return;
        }

        FTransform MyTransform(SourceRotation, SourceLocation, FVector(1.0f));
        AQLRecyclerGrenadeProjectile* RecyclerGrenade = World->SpawnActorDeferred<AQLRecyclerGrenadeProjectile>(RecyclerGrenadeProjectileClass, MyTransform);

        // pass controller to RecyclerGrenade as damage instigator
        AController* Controller = User->GetController();
        AQLPlayerController* QLPlayerController = Cast<AQLPlayerController>(Controller);
        RecyclerGrenade->SetQLPlayerController(QLPlayerController);
        RecyclerGrenade->SetDamageMultiplier(DamageMultiplier);
        UGameplayStatics::FinishSpawningActor(RecyclerGrenade, MyTransform);

        // change velocity
        FVector FinalVelocity = ProjectileForwardVector * RecyclerGrenade->GetProjectileMovementComponent()->InitialSpeed;
        RecyclerGrenade->GetProjectileMovementComponent()->Velocity = FinalVelocity;
    }
}
