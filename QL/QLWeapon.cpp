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
#include "Kismet/GameplayStatics.h"
#include "QLCharacter.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "QLPlayerController.h"
#include "QLWeaponManager.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLWeapon::AQLWeapon() :
WeaponName("None"),
HitRange(10000.0f),
RateOfFire(1.0f),
bIsFireHeld(false),
WeaponManager(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphereComponent"));
    RootSphereComponent->InitSphereRadius(40.0f);
    RootSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    RootSphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    RootComponent = RootSphereComponent;

    GunSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunSkeletalMeshComponent"));
    GunSkeletalMeshComponent->SetupAttachment(RootComponent);

    MuzzleSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleSceneComponent"));
    MuzzleSceneComponent->SetupAttachment(GunSkeletalMeshComponent);

    BeamComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BeamComponent"));
    BeamComponent->SetupAttachment(MuzzleSceneComponent);

    FireSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireSoundComponent"));
    FireSoundComponent->SetupAttachment(RootComponent);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLWeapon::BeginPlay()
{
	Super::BeginPlay();

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
void AQLWeapon::PlayFireSound(const FName& FireSoundName)
{
    USoundBase** Result = FireSoundList.Find(FireSoundName);
    if (Result)
    {
        USoundBase* Sound = *Result;
        AQLCharacter* User = GetWeaponManager()->GetUser();
        if (Sound && User)
        {
            FireSoundComponent->SetSound(Sound);
            FireSoundComponent->Play(0.0f);

            //// sound played using this function is fire and forget and does not travel with the actor
            //UGameplayStatics::PlaySoundAtLocation(this, Sound, User->GetActorLocation());
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::StopFireSound()
{
    if (FireSoundComponent)
    {
        if (FireSoundComponent->IsPlaying())
        {
            FireSoundComponent->Stop();
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::PlayFireAnimation(const FName& FireAnimationName)
{
    UAnimMontage** Result = FireAnimationList.Find(FireAnimationName);
    if (Result)
    {
        UAnimMontage* Animation = *Result;
        AQLCharacter* User = GetWeaponManager()->GetUser();
        if (Animation && User)
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
USkeletalMeshComponent* AQLWeapon::GetGunSkeletalMeshComponent()
{
    return GunSkeletalMeshComponent;
}

//------------------------------------------------------------
//------------------------------------------------------------
FVector AQLWeapon::GetMuzzleLocation()
{
    return MuzzleSceneComponent->GetComponentLocation();
}

//------------------------------------------------------------
//------------------------------------------------------------
FName AQLWeapon::GetWeaponName()
{
    return WeaponName;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::InflictDamage()
{
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
    return WeaponManager;
}

////------------------------------------------------------------
////------------------------------------------------------------
//AQLCharacter* AQLWeapon::GetUser()
//{
//    return User;
//}
//
////------------------------------------------------------------
////------------------------------------------------------------
//AQLPlayerController* AQLWeapon::GetQLPlayerController()
//{
//    if (!User)
//    {
//        return nullptr;
//    }
//
//    AController* Controller = User->GetController();
//    if (!Controller)
//    {
//        return nullptr;
//    }
//
//    AQLPlayerController* QLPlayerController = Cast<AQLPlayerController>(Controller);
//    if (!QLPlayerController)
//    {
//        return nullptr;
//    }
//
//    return QLPlayerController;
//}
//
////------------------------------------------------------------
////------------------------------------------------------------
//UQLUmgUserWidget* AQLWeapon::GetQLUMG()
//{
//    AQLPlayerController* QLPlayerController = GetQLPlayerController();
//    if (!QLPlayerController)
//    {
//        return nullptr;
//    }
//
//    return QLPlayerController->GetUMG();
//}