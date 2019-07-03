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

//------------------------------------------------------------
//------------------------------------------------------------
AQLNailProjectile::AQLNailProjectile()
{
    RootSphereComponent->SetEnableGravity(false);

    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    StaticMeshComponent->SetEnableGravity(false);

    ProjectileLifeSpan = 30.0f;
    ProjectileSpeed = 1000.0f;
    BlastRadius = 100.0f;
    BlastSpeedChange = 100.0f;
    BasicDamage = 20.0f;
    BasicDamageAdjusted = BasicDamage;
    BlastSpeedChangeSelfDamageScale = 10.0f;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLNailProjectile::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    RootSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AQLNailProjectile::OnBeginOverlapForComponent);
}