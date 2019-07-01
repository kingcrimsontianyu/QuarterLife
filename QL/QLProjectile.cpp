//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "QLUtility.h"
#include "QLCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "QLPlayerController.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLProjectile::AQLProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphereComponent"));
    RootSphereComponent->InitSphereRadius(20.0f);
    RootSphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = RootSphereComponent;

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->UpdatedComponent = RootSphereComponent;
    ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
    ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
    ProjectileMovementComponent->SetVelocityInLocalSpace(FVector(0.0f, 0.0f, 0.0f));

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);
    StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

    ExplosionParticleSystem = nullptr;
    ExplosionSound = nullptr;
    ProjectileLifeSpan = 5.0f;
    ProjectileSpeed = 1000.0f;
    BlastRadius = 400.0f;
    BlastSpeedChange = 1200.0f;
    BasicDamage = 100.0f;
    PlayerController = nullptr;
    DamageMultiplier = 1.0f;
    BasicDamageAdjusted = BasicDamage;
    BlastSpeedChangeSelfDamageScale = 1.25f;
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLProjectile::BeginPlay()
{
    Super::BeginPlay();

    ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
    ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;

    SetLifeSpan(ProjectileLifeSpan);
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLProjectile::OnBeginOverlapForComponent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // This function avoids self direct hit. Such case can happen, for example, when the player is
    // firing the projectile down, and the projectile overlaps with the player's vertical capsule immediately.
    // When such case happens, the function returns so as to ignore the current overlap event.
    // When such case does not happen, this function is guaranteed to be called only once,
    // because even though the projectile may overlap several components,
    // it is destroyed instantly upon the first overlap event.
    if (OtherActor)
    {
        bool bSelfDirectHit = false;
        bool bDirectHit = false;
        HandleDirectHit(OtherActor, bSelfDirectHit, bDirectHit);
        if (bSelfDirectHit)
        {
            return;
        }

        HandleSplashHit(OtherActor, bDirectHit);

        Destroy();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLProjectile::HandleDirectHit(AActor* OtherActor, bool& bSelfDirectHit, bool& bDirectHit)
{
    bDirectHit = false;
    bSelfDirectHit = false;

    AQLCharacter* Character = Cast<AQLCharacter>(OtherActor);
    if (Character)
    {
        // prevent self direct hit from happening
        if (PlayerController.IsValid() && OtherActor == PlayerController->GetCharacter())
        {
            bSelfDirectHit = true;
            return;
        }

        float DamageAmount = BasicDamageAdjusted;
        const FPointDamageEvent DamageEvent;
        DamageAmount = Character->TakeDamage(DamageAmount, DamageEvent, PlayerController.Get(), this);

        // display damage
        if (DamageAmount > 0.0f && PlayerController.IsValid())
        {
            PlayerController->ShowDamageOnScreen(DamageAmount, Character->GetActorLocation());
        }

        bDirectHit = true;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLProjectile::HandleSplashHit(AActor* OtherActor, bool bDirectHit)
{
    // get victims within the blast radius
    FVector Epicenter = GetActorLocation();
    TArray<FOverlapResult> OutOverlaps;
    FCollisionObjectQueryParams CollisionObjectQueryParams(ECollisionChannel::ECC_Pawn);
    FCollisionQueryParams CollisionQueryParams;

    GetWorld()->OverlapMultiByObjectType(OutOverlaps,
        Epicenter,
        FQuat(GetActorRotation()),
        CollisionObjectQueryParams,
        FCollisionShape::MakeSphere(BlastRadius),
        CollisionQueryParams);

    // iterate victims
    for (auto&& Result : OutOverlaps)
    {
        TWeakObjectPtr<UPrimitiveComponent> Comp = Result.Component;
        AActor* Actor = Comp->GetOwner();
        if (Actor)
        {
            AQLCharacter* Character = Cast<AQLCharacter>(Actor);
            if (Character)
            {
                // change victim velocity
                UCharacterMovementComponent*  CharacterMovementComponent = Character->GetCharacterMovement();
                if (CharacterMovementComponent)
                {
                    // a less good approach is LaunchCharacter()
                    // Character->LaunchCharacter(FVector(0.0f, 0.0f, 100.0f), true, true);

                    float ActualBlastSpeedChange = BlastSpeedChange;
                    if (PlayerController.IsValid() && Character == PlayerController->GetCharacter())
                    {
                        ActualBlastSpeedChange *= BlastSpeedChangeSelfDamageScale;
                    }

                    CharacterMovementComponent->AddRadialImpulse(
                        GetActorLocation(),
                        BlastRadius,
                        ActualBlastSpeedChange,
                        ERadialImpulseFalloff::RIF_Linear,
                        true); // velocity change (true) or impulse (false)
                }

                // inflict damage
                // if direct hit damage has already been applied, skip to the next victim
                if (bDirectHit && OtherActor == Actor)
                {
                    continue;
                }

                // reduce self splash damage (e.g. rocket jump)
                float DamageAmount = BasicDamageAdjusted;

                if (PlayerController.IsValid() && Character == PlayerController->GetCharacter())
                {
                    DamageAmount = ReduceSelfDamage(DamageAmount);
                }

                FRadialDamageEvent DamageEvent;
                DamageEvent.Params.BaseDamage = DamageAmount;
                DamageEvent.Params.OuterRadius = BlastRadius;
                DamageEvent.Params.MinimumDamage = 0.0;

                DamageAmount = Character->TakeDamage(DamageAmount, DamageEvent, PlayerController.Get(), this);

                // display positive damage
                if (DamageAmount > 0.0f && PlayerController.IsValid())
                {
                    PlayerController->ShowDamageOnScreen(DamageAmount, Character->GetActorLocation());
                }
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
UProjectileMovementComponent* AQLProjectile::GetProjectileMovementComponent()
{
    return ProjectileMovementComponent;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
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

//------------------------------------------------------------
//------------------------------------------------------------
void AQLProjectile::SetQLPlayerController(AQLPlayerController* PlayerControllerExt)
{
    PlayerController = PlayerControllerExt;
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLProjectile::ReduceSelfDamage(const float InDamage)
{
    return InDamage * 0.5;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLProjectile::SetDamageMultiplier(const float Value)
{
    DamageMultiplier = Value;

    BasicDamageAdjusted = Value * BasicDamage;
}