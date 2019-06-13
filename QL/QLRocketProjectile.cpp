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

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLRocketProjectile::AQLRocketProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RocketLifeSpan = 5.0f;
    RocketSpeed = 2000.0f;

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

    ExplosionParticleSystem = nullptr;
    ExplosionSound = nullptr;
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLRocketProjectile::BeginPlay()
{
	Super::BeginPlay();

    ProjectileMovementComponent->InitialSpeed = RocketSpeed;
    ProjectileMovementComponent->MaxSpeed = RocketSpeed;

    SetLifeSpan(RocketLifeSpan);
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
    QLUtility::Log(OtherActor->GetName());

    // Only add impulse and destroy projectile if we hit a physics
    if (OtherActor)
    {
        AQLCharacter* Character = Cast<AQLCharacter>(OtherActor);
        if (Character)
        {
            Character->LaunchCharacter(FVector(0.0f, 0.0f, 100.0f), true, true);
        }

        //QLUtility::Log(OtherComp->GetClass()->GetFName().ToString());
        //if (OtherComp && OtherComp->IsSimulatingPhysics())
        //{
        //    OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
        //}

        Destroy();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
UProjectileMovementComponent* AQLRocketProjectile::GetProjectileMovementComponent()
{
    return ProjectileMovementComponent;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLRocketProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (EndPlayReason == EEndPlayReason::Destroyed)
    {
        // play explosion particle system
        if (ExplosionParticleSystem)
        {
            FTransform Transform(FRotator::ZeroRotator,
                GetActorLocation(),
                FVector(4.0f)); // scale

            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
                ExplosionParticleSystem,
                Transform,
                true, // auto destroy
                EPSCPoolMethod::AutoRelease);
        }

        // play explosion sound
        if (ExplosionSound)
        {
            // fire and forget
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
        }
    }
}