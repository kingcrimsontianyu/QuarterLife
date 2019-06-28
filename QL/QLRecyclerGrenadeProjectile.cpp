//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLRecyclerGrenadeProjectile.h"
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
AQLRecyclerGrenadeProjectile::AQLRecyclerGrenadeProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphereComponent"));
    RootSphereComponent->InitSphereRadius(10.0f);
    RootSphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    RootComponent = RootSphereComponent;

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->UpdatedComponent = RootSphereComponent;
    ProjectileMovementComponent->InitialSpeed = GrenadeSpeed;
    ProjectileMovementComponent->MaxSpeed = GrenadeSpeed;
    ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->SetVelocityInLocalSpace(FVector(0.0f, 0.0f, 0.0f));
    ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &AQLRecyclerGrenadeProjectile::OnProjectileBounceImpl);

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);
    StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
    StaticMeshComponent->SetEnableGravity(false);

    PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));

    GrenadeSpeed = 2000.0f;
    PlayerController = nullptr;

    BasicDamage = 200.0f;
    DamageMultiplier = 1.0f;
    BasicDamageAdjusted = BasicDamage;

    IdleDuration = 2.5f;
    AttractDuration = 1.5f;
    BlastRadius = 400.0f;
    BlastSpeedChange = 1200.0f;

    BounceSound = nullptr;
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();

    ProjectileMovementComponent->InitialSpeed = GrenadeSpeed;
    ProjectileMovementComponent->MaxSpeed = GrenadeSpeed;

    GetWorldTimerManager().SetTimer(IdleTimerHandle,
        this,
        &AQLRecyclerGrenadeProjectile::Implode,
        IdleDuration, // time interval in second
        false, // loop
        0.0f); // delay in second
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (PostProcessComponent)
    {
        if (MaterialSpaceWarp)
        {
            FWeightedBlendable WeightedBlendable;
            DynamicMaterialSpaceWarp = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, MaterialSpaceWarp);
            WeightedBlendable.Object = DynamicMaterialSpaceWarp.Get();
            WeightedBlendable.Weight = 1.0f;
            PostProcessComponent->Settings.WeightedBlendables.Array.Add(WeightedBlendable);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
UProjectileMovementComponent* AQLRecyclerGrenadeProjectile::GetProjectileMovementComponent()
{
    return ProjectileMovementComponent;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::SetQLPlayerController(AQLPlayerController* PlayerControllerExt)
{
    PlayerController = PlayerControllerExt;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::SetDamageMultiplier(const float Value)
{
    DamageMultiplier = Value;

    BasicDamageAdjusted = Value * BasicDamage;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::OnProjectileBounceImpl(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
    // play explosion sound
    if (BounceSound)
    {
        // fire and forget
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), BounceSound, ImpactResult.ImpactPoint);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::Implode()
{

}
