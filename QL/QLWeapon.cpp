//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLWeapon.h"
#include "Components/SphereComponent.h"
#include "QLCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"
#include "QLPlayerController.h"
#include "QLWeaponManager.h"
#include "Kismet/GameplayStatics.h"
#include "QLUtility.h"
#include "TimerManager.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLWeapon::AQLWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    QLName = "None";
    HitRange = 10000.0f;
    RateOfFire = 1.0f;
    bIsFireHeld = false;
    bFireEnabled = true;

    GunSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunSkeletalMeshComponent"));
    GunSkeletalMeshComponent->SetupAttachment(RootComponent);
    GunSkeletalMeshComponent->SetSimulatePhysics(false);
    GunSkeletalMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
    GunSkeletalMeshComponent->CastShadow = true;
    GunSkeletalMeshComponent->bCastDynamicShadow = true;

    MuzzleSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleSceneComponent"));
    MuzzleSceneComponent->SetupAttachment(GunSkeletalMeshComponent);

    BeamComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BeamComponent"));
    BeamComponent->SetupAttachment(MuzzleSceneComponent);

    DamageMultiplier = 1.0;

    bIsProjectileWeapon = false;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (GunSkeletalMeshComponent)
    {
        UMaterialInterface* BasicMaterial = GunSkeletalMeshComponent->GetMaterial(0);
        DynamicMaterialGun = GunSkeletalMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BasicMaterial);
        GunSkeletalMeshComponent->SetMaterial(0, DynamicMaterialGun.Get());
    }

    AnimInstanceWeapon = GunSkeletalMeshComponent->GetAnimInstance();
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLWeapon::BeginPlay()
{
	Super::BeginPlay();

}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    // to do: under which EndPlayReason?
    GetWorldTimerManager().ClearAllTimersForObject(this);
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::OnFire()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::OnFireRelease()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::OnFireHold()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::OnAltFire()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::OnAltFireRelease()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::OnAltFireHold()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::SpawnProjectile()
{

}

//------------------------------------------------------------
//------------------------------------------------------------
USkeletalMeshComponent* AQLWeapon::GetGunSkeletalMeshComponent()
{
    return GunSkeletalMeshComponent;
}

//------------------------------------------------------------
//------------------------------------------------------------
FVector AQLWeapon::GetMuzzleLocation()
{
    if (MuzzleSceneComponent)
    {
        return MuzzleSceneComponent->GetComponentLocation();
    }
    else
    {
        return FVector();
    }

}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::SetHitRange(float HitRangeExt)
{
    HitRange = HitRangeExt;
}

//------------------------------------------------------------
//------------------------------------------------------------
UTexture2D* AQLWeapon::GetCrossHairTexture(const FName& CrossHairTextureName)
{
    UTexture2D* Result = nullptr;

    UTexture2D** Temp = CrosshairTextureList.Find(CrossHairTextureName);
    if (Temp)
    {
        Result = *Temp;
    }

    return Result;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::SetWeaponManager(UQLWeaponManager* WeaponManagerExt)
{
    WeaponManager = WeaponManagerExt;
}

//------------------------------------------------------------
//------------------------------------------------------------
UQLWeaponManager* AQLWeapon::GetWeaponManager()
{
    return WeaponManager.Get();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::StopFire()
{
    // stop firing
    if (bIsFireHeld)
    {
        OnFireRelease();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AQLCharacter* QLCharacter = Cast<AQLCharacter>(OtherActor);
    if (QLCharacter)
    {
        // if the character has weapon of this type already, nothing will happen
        if (QLCharacter->HasWeapon(this->GetQLName()))
        {
            return;
        }

        QLCharacter->AddWeapon(this);
        QLCharacter->SetCurrentWeapon(this->GetQLName());
        PlaySound("PickUp");

        // disable delegate
        if (RootSphereComponent)
        {
            RootSphereComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AQLWeapon::OnComponentBeginOverlapImpl);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::EnableFireCallBack()
{
    bFireEnabled = true;

    GetWorldTimerManager().ClearTimer(DisableFireTimerHandle);
}

//------------------------------------------------------------
//------------------------------------------------------------
UAnimMontage* AQLWeapon::GetAnimationMontage(const FName& MontageName)
{
    UAnimMontage* MyMontage = nullptr;

    UAnimMontage** Result = AnimationMontageList.Find(MontageName);
    if (Result)
    {
        MyMontage = *Result;
    }

    return MyMontage;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::PlayAnimationMontage(const FName& MontageName)
{
    UAnimMontage** Result = AnimationMontageList.Find(MontageName);
    if (Result)
    {
        UAnimMontage* MyAnimationMontage = *Result;
        if (MyAnimationMontage)
        {
            UAnimInstance* AnimInstance = GunSkeletalMeshComponent->GetAnimInstance();
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(MyAnimationMontage, 1.0f);
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::PlayAnimationMontageJumpToSection(const FName& MontageName, const FName& SectionName)
{
    UAnimMontage** Result = AnimationMontageList.Find(MontageName);
    if (Result)
    {
        UAnimMontage* MyAnimationMontage = *Result;
        if (MyAnimationMontage)
        {
            UAnimInstance* AnimInstance = GunSkeletalMeshComponent->GetAnimInstance();
            if (AnimInstance)
            {
                AnimInstance->Montage_JumpToSection(SectionName, MyAnimationMontage);
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::PlayAnimationMontageForCharacter(const FName& MontageName)
{
    UAnimMontage** Result = AnimationMontageList.Find(MontageName);
    if (Result)
    {
        UAnimMontage* Animation = *Result;
        if (Animation && WeaponManager.IsValid())
        {
            AQLCharacter* User = WeaponManager->GetUser();
            if (User)
            {
                USkeletalMeshComponent* ArmMesh = User->GetFirstPersonMesh();
                if (ArmMesh)
                {
                    UAnimInstance* AnimInstance = ArmMesh->GetAnimInstance();
                    if (AnimInstance)
                    {
                        AnimInstance->Montage_Play(Animation, 1.0f);
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::SetDamageMultiplier(float Value)
{
    DamageMultiplier = Value;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::StartGlow(const FLinearColor& Color)
{
    if (GunSkeletalMeshComponent && DynamicMaterialGun.IsValid())
    {
        DynamicMaterialGun->SetScalarParameterValue("GlowEnabled", 1.0f);
        DynamicMaterialGun->SetVectorParameterValue("GlowColor", Color);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::StopGlow()
{
    if (GunSkeletalMeshComponent && DynamicMaterialGun.IsValid())
    {
        DynamicMaterialGun->SetScalarParameterValue("GlowEnabled", 0.0f);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLWeapon::GetKnockbackSpeedChange()
{
    return KnockbackSpeedChange;
}

//------------------------------------------------------------
//------------------------------------------------------------
bool AQLWeapon::IsProjectileWeapon()
{
    return bIsProjectileWeapon;
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLWeapon::GetProjectileSpeed()
{
    return ProjectileSpeed;
}
