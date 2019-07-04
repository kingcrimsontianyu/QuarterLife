//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLPickup.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
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
AQLPickup::AQLPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphereComponent"));
    RootSphereComponent->InitSphereRadius(40.0f);
    RootSphereComponent->SetSimulatePhysics(false);
    RootSphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = RootSphereComponent;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);
    StaticMeshComponent->SetSimulatePhysics(false);
    StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

    SoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundComponent"));
    SoundComponent->SetupAttachment(RootComponent);

    bCanBeRespawned = false;
    bConstantlyRotating = true;
    RespawnInterval = 0.0f;
    GlowColor = FLinearColor(0.0f, 0.0f, 1.0f);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLPickup::BeginPlay()
{
	Super::BeginPlay();

    SetConstantRotationEnabled(true);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPickup::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorldTimerManager().ClearAllTimersForObject(this);
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // constant rotation
    if (bConstantlyRotating)
    {
        // derivation:
        // rotation rate [degree / sec] = angle per tick / time per tick
        // so angle per tick = rotation rate * time per tick

        FRotator Increment(RotationRate.Pitch * DeltaTime, RotationRate.Yaw * DeltaTime, RotationRate.Roll * DeltaTime);
        FRotator Rotator = GetActorRotation();
        Rotator.Add(Increment.Pitch, Increment.Yaw, Increment.Roll);
        SetActorRotation(Rotator);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPickup::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (StaticMeshComponent)
    {
        UMaterialInterface* BasicMaterial = StaticMeshComponent->GetMaterial(0);
        DynamicMaterial = StaticMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BasicMaterial);
        StaticMeshComponent->SetMaterial(0, DynamicMaterial.Get());
    }

    if (RootSphereComponent)
    {
        RootSphereComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AQLPickup::OnComponentBeginOverlapImpl);
        RootSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AQLPickup::OnComponentBeginOverlapImpl);
    }
}


//------------------------------------------------------------
//------------------------------------------------------------
void AQLPickup::PlayAnimationMontage(const FName& AnimationMontageName)
{
}


//------------------------------------------------------------
//------------------------------------------------------------
void AQLPickup::PlaySoundFireAndForget(const FName& SoundName)
{
    USoundBase** Result = SoundList.Find(SoundName);
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
void AQLPickup::PlaySound(const FName& SoundName)
{
    USoundBase** Result = SoundList.Find(SoundName);
    if (Result)
    {
        USoundBase* Sound = *Result;
        if (Sound)
        {
            SoundComponent->SetSound(Sound);
            SoundComponent->Play(0.0f);

            //// sound played using this function is fire and forget and does not travel with the actor
            //UGameplayStatics::PlaySoundAtLocation(this, Sound, User->GetActorLocation());
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPickup::StopSound()
{
    if (SoundComponent)
    {
        if (SoundComponent->IsPlaying())
        {
            SoundComponent->Stop();
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPickup::SetConstantRotationEnabled(const bool bFlag)
{
    bConstantlyRotating = bFlag;
}

//------------------------------------------------------------
//------------------------------------------------------------
USphereComponent* AQLPickup::GetRootSphereComponent()
{
    return RootSphereComponent;
}

//------------------------------------------------------------
//------------------------------------------------------------
FLinearColor AQLPickup::GetGlowColor()
{
    return GlowColor;
}

//------------------------------------------------------------
//------------------------------------------------------------
FName AQLPickup::GetQLName()
{
    return QLName;
}

//------------------------------------------------------------
//------------------------------------------------------------
UStaticMeshComponent* AQLPickup::GetStaticMeshComponent()
{
    return StaticMeshComponent;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPickup::OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}