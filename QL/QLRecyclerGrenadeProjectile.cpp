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
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "QLPlayerController.h"
#include "QLCharacter.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLRecyclerGrenadeProjectile::AQLRecyclerGrenadeProjectile()
{
    RootSphereComponent->InitSphereRadius(10.0f);
    RootSphereComponent->SetEnableGravity(false);
    RootSphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));

    ProjectileMovementComponent->bShouldBounce = true;

    StaticMeshComponent->SetEnableGravity(false);

    PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
    PostProcessComponent->bEnabled = false;

    ProjectileSpeed = 2000.0f;
    PlayerController = nullptr;

    BasicDamage = 200.0f;
    DamageMultiplier = 1.0f;
    BasicDamageAdjusted = BasicDamage;

    IdleDuration = 2.5f;
    AttractDuration = 1.5f;
    AttractInterval = 0.01f;
    RecoverDuration = 2.0f;
    BlastRadius = 400.0f;
    BlastSpeedChange = 1200.0f;

    bCalculateMaterialParameter = false;

    // animation
    SpaceWarpTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SpaceWarpTimeline"));
    SpaceWarpTimelineInterpFunction.BindUFunction(this, FName(TEXT("SpaceWarpCallback")));
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();

    GetWorldTimerManager().SetTimer(IdleTimerHandle,
        this,
        &AQLRecyclerGrenadeProjectile::Implode,
        1.0f, // time interval in second
        false, // loop
        IdleDuration); // delay in second
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bCalculateMaterialParameter)
    {
        CalculateMaterialParameter();
    }
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

    if (SpaceWarpTimeline && SpaceWarpCurve)
    {
        SpaceWarpTimeline->AddInterpFloat(SpaceWarpCurve, SpaceWarpTimelineInterpFunction, FName(TEXT("SpaceWarp")));
    }

    if (ProjectileMovementComponent)
    {
        ProjectileMovementComponent->OnProjectileBounce.RemoveDynamic(this, &AQLRecyclerGrenadeProjectile::OnProjectileBounceImpl);
        ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &AQLRecyclerGrenadeProjectile::OnProjectileBounceImpl);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::OnProjectileBounceImpl(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
    PlaySoundFireAndForget("Bounce");
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::Implode()
{
    PostProcessComponent->bEnabled = true;

    bCalculateMaterialParameter = true;

    if (SpaceWarpTimeline)
    {
        SpaceWarpTimeline->PlayFromStart();
    }

    GetWorldTimerManager().SetTimer(AttractTimerHandle,
        this,
        &AQLRecyclerGrenadeProjectile::Attract,
        AttractInterval, // time interval in second
        true, // loop
        0.0f); // delay in second

    GetWorldTimerManager().SetTimer(ImplodeTimerHandle,
        this,
        &AQLRecyclerGrenadeProjectile::Annihilate,
        1.0f, // time interval in second
        false, // loop
        AttractDuration); // delay in second
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::Attract()
{

}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::Annihilate()
{
    StaticMeshComponent->SetVisibility(false);

    GetWorldTimerManager().ClearTimer(AttractTimerHandle);

    SetLifeSpan(RecoverDuration);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::CalculateMaterialParameter()
{
    if (PlayerController.IsValid())
    {
        // convert coordinates
        FVector Epicenter = GetActorLocation();
        FVector2D ScreenLocation;

        bool bIsConversionSuccessful = PlayerController->ProjectWorldLocationToScreen(Epicenter, ScreenLocation);
        if (!bIsConversionSuccessful)
        {
            return;
        }

        if (DynamicMaterialSpaceWarp.IsValid())
        {
            int32 X, Y;
            PlayerController->GetViewportSize(X, Y);

            FVector Temp;
            Temp.X = ScreenLocation.X / X;
            Temp.Y = ScreenLocation.Y / Y;
            DynamicMaterialSpaceWarp->SetVectorParameterValue("Center0", Temp);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLRecyclerGrenadeProjectile::SpaceWarpCallback(float Value)
{
    if (DynamicMaterialSpaceWarp.IsValid())
    {
        DynamicMaterialSpaceWarp->SetScalarParameterValue("CollapseLevel0", Value);

        // determine the appropriate visual blast radius
        if (!PlayerController.IsValid())
        {
            return;
        }

        APawn* Pawn = PlayerController->GetPawn();
        if (!Pawn)
        {
            return;
        }

        AQLCharacter* QLCharacter = Cast<AQLCharacter>(Pawn);
        if (!QLCharacter)
        {
            return;
        }

        UCameraComponent* Camera = QLCharacter->GetFirstPersonCameraComponent();
        if (!Camera)
        {
            return;
        }

        FVector CameraLocation = Camera->GetComponentLocation();
        FVector Epicenter = GetActorLocation();
        float Distance = FVector::Distance(CameraLocation, Epicenter);

        constexpr float InitialRadius = 0.8f;
        constexpr float InitialDistance = 200.0f;
        float Radius = InitialRadius;

        if (Distance > InitialDistance)
        {
            Radius = InitialRadius * InitialDistance / Distance;
        }

        DynamicMaterialSpaceWarp->SetScalarParameterValue("Radius0", Radius);
    }
}
