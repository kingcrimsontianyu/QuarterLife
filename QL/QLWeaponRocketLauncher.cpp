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
#include "QLRocketProjectile.h"
#include "QLWeaponManager.h"
#include "QLCharacter.h"
#include "QLUtility.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "QLRocketProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "QLPlayerController.h"
#include "Kismet/GameplayStatics.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponRocketLauncher::AQLWeaponRocketLauncher()
{
    QLName = FName(TEXT("RocketLauncher"));
    RateOfFire = 0.8f;
    RocketProjectileClass = AQLRocketProjectile::StaticClass();
    bIsProjectileWeapon = true;
    ProjectileSpeed = 2500.0f; // 1000 ups = 2500 cm/s
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRocketLauncher::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    SetDamageMultiplier(1.0f);
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

    FVector SourceLocation = CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * 200.0f;
    FVector TargetLocation;

    // if hit occurs
    if (HitResult.bBlockingHit)
    {
        TargetLocation = HitResult.ImpactPoint;
    }
    else
    {
        TargetLocation = SourceLocation + CameraComponent->GetForwardVector() * HitRange;
    }

    FVector ProjectileForwardVector = TargetLocation - SourceLocation;
    ProjectileForwardVector.Normalize();

    FMatrix result = FRotationMatrix::MakeFromXZ(ProjectileForwardVector, CameraComponent->GetUpVector());
    FRotator SourceRotation = result.Rotator();

    // spawn and launch a rocket
    UWorld* const World = GetWorld();

    if (RocketProjectileClass && World && WeaponManager.IsValid())
    {
        if (!User || !CameraComponent)
        {
            return;
        }

        FTransform MyTransform(SourceRotation, SourceLocation, FVector(1.0f));
        AQLRocketProjectile* Rocket = World->SpawnActorDeferred<AQLRocketProjectile>(RocketProjectileClass, MyTransform);

        // pass controller to rocket as damage instigator
        AController* Controller = User->GetController();
        AQLPlayerController* QLPlayerController = Cast<AQLPlayerController>(Controller);
        Rocket->QLSetPlayerController(QLPlayerController);
        Rocket->SetDamageMultiplier(DamageMultiplier);
        UGameplayStatics::FinishSpawningActor(Rocket, MyTransform);

        // change velocity
        Rocket->GetProjectileMovementComponent()->MaxSpeed = ProjectileSpeed;
        Rocket->GetProjectileMovementComponent()->InitialSpeed = ProjectileSpeed;
        FVector FinalVelocity = ProjectileForwardVector * ProjectileSpeed;
        Rocket->GetProjectileMovementComponent()->Velocity = FinalVelocity;
    }
}