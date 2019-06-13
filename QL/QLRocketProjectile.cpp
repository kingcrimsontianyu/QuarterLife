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

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLRocketProjectile::AQLRocketProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RocketLifeSpan = 10.0f;
    RocketSpeed = 1000.0f;

    RootSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphereComponent"));
    RootSphereComponent->InitSphereRadius(20.0f);
    RootSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    RootSphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    RootSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AQLRocketProjectile::OnBeginOverlapForComponent);
    RootSphereComponent->SetEnableGravity(false);
    RootComponent = RootSphereComponent;

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->UpdatedComponent = RootSphereComponent;
    ProjectileMovementComponent->InitialSpeed = RocketSpeed;
    ProjectileMovementComponent->MaxSpeed = RocketSpeed;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    ProjectileMovementComponent->SetVelocityInLocalSpace(FVector(.0f, 0.0f, 0.0f));

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);
    StaticMeshComponent->SetSimulatePhysics(false);
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    StaticMeshComponent->SetEnableGravity(false);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLRocketProjectile::BeginPlay()
{
	Super::BeginPlay();

    // property rocket speed may be modified in BP
    ProjectileMovementComponent->InitialSpeed = RocketSpeed;
    ProjectileMovementComponent->MaxSpeed = RocketSpeed;
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLRocketProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLRocketProjectile::OnBeginOverlapForComponent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Only add impulse and destroy projectile if we hit a physics
    if (OtherActor)
    {
        if (OtherComp && OtherComp->IsSimulatingPhysics())
        {
            OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
        }

        Destroy();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
UProjectileMovementComponent* AQLRocketProjectile::GetProjectileMovementComponent()
{
    return ProjectileMovementComponent;
}