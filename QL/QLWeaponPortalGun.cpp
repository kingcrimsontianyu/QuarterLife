//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLWeaponPortalGun.h"
#include "QLCharacter.h"
#include "QLUtility.h"
#include "QLColoredPortal.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "QLPortalCompatibleActor.h"
#include "Kismet/GameplayStatics.h"
#include "QLWeaponManager.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLWeaponPortalGun::AQLWeaponPortalGun()
{
    HitRange = 10000.0f;

    QLName = FName("PortalGun");

    PortalClass = AQLColoredPortal::StaticClass();
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLWeaponPortalGun::BeginPlay()
{
	Super::BeginPlay();

}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLWeaponPortalGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::OnFire()
{
    CreatePortalIfConditionsAreMet(EPortalColor::Blue);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::OnAltFire()
{
    CreatePortalIfConditionsAreMet(EPortalColor::Orange);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::CreatePortalIfConditionsAreMet(EPortalColor PortalColor)
{
    if (!WeaponManager.IsValid())
    {
        return;
    }

    AQLCharacter* User = GetWeaponManager()->GetUser();
    if (User == nullptr)
    {
        return;
    }

    FHitResult HitResult = User->RayTraceFromCharacterPOV(HitRange);

    // if hit does not occur
    if (!HitResult.bBlockingHit)
    {
        // do sth
        QLUtility::Log("AQLWeaponPortalGun: no hit");
        PlaySound(FName("NoPortal"));
        return;
    }

    // if hit occurs
    // check if the hit actor is compatible with the portal gun
    auto* pgcActor = Cast<AQLPortalCompatibleActor>(HitResult.GetActor());

    // if the hit actor is not compatible
    if (!pgcActor)
    {
        // do sth
        QLUtility::Log("AQLWeaponPortalGun: not compatible");
        PlaySound(FName("NoPortal"));
        return;
    }

    // remove portal of the same color generated elsewhere
    if (PortalColor == EPortalColor::Blue)
    {
        if (BluePortal.IsValid())
        {
            BluePortal->CleanUp();
            BluePortal->Destroy();
            BluePortal = nullptr;
        }
    }
    else
    {
        if (OrangePortal.IsValid())
        {
            OrangePortal->CleanUp();
            OrangePortal->Destroy();
            OrangePortal = nullptr;
        }
    }

    // all conditions are met, now spawn the portal
    FVector location = HitResult.ImpactPoint;
    FMatrix result = FRotationMatrix::MakeFromXZ(HitResult.Normal, pgcActor->GetActorUpVector());
    FRotator rotation = result.Rotator();
    FTransform transform;

    // if the hit actor is compatible
    AQLColoredPortal* Portal = GetWorld()->SpawnActorDeferred<AQLColoredPortal>(PortalClass, transform);
    // budge the hitbox a wee bit so that the portal can be displayed properly
    location += (Portal->GetBoxComponent()->GetScaledBoxExtent().X + 1.0f) * HitResult.Normal;
    Portal->SetActorLocation(location);
    Portal->SetActorRotation(rotation);
    UGameplayStatics::FinishSpawningActor(Portal, transform);

    // the newly spawned portal has top priority
    // previously spawned overlapping portal is destroyed
    TSet<AActor*> OverlapperList;
    Portal->GetOverlappingActors(OverlapperList, AQLColoredPortal::StaticClass());
    if (OverlapperList.Num() > 0)
    {
        for (const auto& Item : OverlapperList)
        {
            if (Item == BluePortal)
            {
                BluePortal->CleanUp();
                BluePortal->Destroy();
                BluePortal = nullptr;
            }
            else
            {
                OrangePortal->CleanUp();
                OrangePortal->Destroy();
                OrangePortal = nullptr;
            }
        }
    }

    // now that a new portal is appropriately created without overlap,
    // set the new portal's properties
    if (PortalColor == EPortalColor::Blue)
    {
        Portal->Initialize(EPortalColor::Blue, OrangePortal.Get());
        BluePortal = Portal;
    }
    else
    {
        Portal->Initialize(EPortalColor::Orange, BluePortal.Get());
        OrangePortal = Portal;
    }

    // sound
    FName SoundName;
    if (PortalColor == EPortalColor::Blue)
    {
        SoundName = FName("FireBlue");
    }
    else
    {
        SoundName = FName("FireOrange");
    }
    PlaySound(SoundName);
    PlayAnimationMontage(FName("Fire"));
}

