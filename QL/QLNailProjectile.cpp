//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLNailProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "QLUtility.h"
#include "QLCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "QLPlayerController.h"
#include "Engine/DecalActor.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLNailProjectile::AQLNailProjectile()
{
    RootSphereComponent->SetEnableGravity(false);

    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    StaticMeshComponent->SetEnableGravity(false);

    ProjectileLifeSpan = 30.0f;
    ProjectileSpeed = 1500.0f;
    BlastRadius = 50.0f;
    BlastSpeedChange = 100.0f;
    BasicDamage = 15.0f;
    BasicDamageAdjusted = BasicDamage;
    BlastSpeedChangeSelfDamageScale = 10.0f;
    ExplosionParticleSystemScale = 0.4f;
    DecalClass = ADecalActor::StaticClass();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLNailProjectile::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    RootSphereComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AQLNailProjectile::OnBeginOverlapForComponent);
    RootSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AQLNailProjectile::OnBeginOverlapForComponent);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLNailProjectile::OnBeginOverlapForComponent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // create bullet hole decal, if the hit actor is not a character
    if (OtherActor)
    {
        AQLCharacter* Character = Cast<AQLCharacter>(OtherActor);
        if (!Character)
        {
            FMatrix RotationMatrix = FRotationMatrix::MakeFromZ(SweepResult.ImpactNormal);
            FRotator Rotation = RotationMatrix.Rotator();
            ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(DecalClass, SweepResult.ImpactPoint, Rotation);
        }
    }

    // randomly sample from one of 3 nail gun hit sound effects to play
    int32 Min = 1;
    int32 Max = 3;
    int32 Idx = FMath::RandRange(Min, Max);
    FString SoundName = "NailGunHit" + FString::FromInt(Idx);
    PlaySoundFireAndForget(FName(*SoundName));

    Super::OnBeginOverlapForComponent(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}