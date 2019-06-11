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

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLWeapon::AQLWeapon() :
WeaponName("None"),
User(nullptr),
HitRange(10000.0f),
RateOfFire(1.0f),
bIsFireHeld(false)
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
void AQLWeapon::SetUser(AQLCharacter* Character)
{
    User = Character;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::UnsetUser()
{
    User = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::PlayFireSound(const FName& FireSoundName)
{
    USoundBase** Result = FireSoundList.Find(FireSoundName);
    if (Result)
    {
        USoundBase* Sound = *Result;
        if (Sound && User)
        {
            // sound played using this function is fire and forget and does not travel with the actor
            UGameplayStatics::PlaySoundAtLocation(this, Sound, User->GetActorLocation());
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
void AQLWeapon::Fire()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::FireRelease()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::FireRepeat()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::AltFire()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::AltFireRelease()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::AltFireRepeat()
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