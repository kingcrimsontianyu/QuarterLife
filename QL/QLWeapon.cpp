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

    WeaponName = "None";
    HitRange = 10000.0f;
    RateOfFire = 1.0f;
    bIsFireHeld = false;
    bFireEnabled = true;

    RootSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphereComponent"));
    RootSphereComponent->InitSphereRadius(40.0f);
    RootSphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    RootSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AQLWeapon::OnComponentBeginOverlapImpl);
    RootComponent = RootSphereComponent;

    GunSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunSkeletalMeshComponent"));
    GunSkeletalMeshComponent->SetupAttachment(RootComponent);
    GunSkeletalMeshComponent->SetSimulatePhysics(false);
    GunSkeletalMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
    GunSkeletalMeshComponent->CastShadow = false;

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
void AQLWeapon::PlayFireSoundFireAndForget(const FName& FireSoundName)
{
    USoundBase** Result = FireSoundList.Find(FireSoundName);
    if (Result)
    {
        USoundBase* Sound = *Result;
        if (Sound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::PlayFireSound(const FName& FireSoundName)
{
    USoundBase** Result = FireSoundList.Find(FireSoundName);
    if (Result)
    {
        USoundBase* Sound = *Result;
        if (Sound)
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
FName AQLWeapon::GetWeaponName()
{
    return WeaponName;
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
void AQLWeapon::PrepareForImpendingWeaponSwitch()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AQLCharacter* QLCharacter = Cast<AQLCharacter>(OtherActor);
    if (QLCharacter)
    {
        QLCharacter->AddWeapon(this);
        QLCharacter->SetCurrentWeapon(this->GetWeaponName());
        PlayFireSound("PickUp");
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::EnableFireCallBack()
{
    bFireEnabled = true;

    GetWorldTimerManager().ClearTimer(DisableFireTimerHandle);
}
