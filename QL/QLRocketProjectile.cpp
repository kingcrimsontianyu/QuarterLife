//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLRocketProjectile.h"
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
// Sets default values
//------------------------------------------------------------
AQLRocketProjectile::AQLRocketProjectile()
{
    RootSphereComponent->SetEnableGravity(false);
    RootSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AQLRocketProjectile::OnBeginOverlapForComponent);

    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    StaticMeshComponent->SetEnableGravity(false);

    ProjectileLifeSpan = 5.0f;
    ProjectileSpeed = 2000.0f;
    BlastRadius = 400.0f;
    BlastSpeedChange = 1200.0f;
    BasicDamage = 100.0f;
    BasicDamageAdjusted = BasicDamage;
    BlastSpeedChangeSelfDamageScale = 1.25f;
}
