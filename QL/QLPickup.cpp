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

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->InitSphereRadius(40.0f);
    SphereComponent->SetSimulatePhysics(false);
    SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = SphereComponent;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);
    StaticMeshComponent->SetSimulatePhysics(false);
    StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

    SoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundComponent"));
    SoundComponent->SetupAttachment(RootComponent);

    bCanBeRespawned = false;
    RespawnInterval = 0.0f;
    GlowColor = FLinearColor(0.0f, 0.0f, 1.0f);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLPickup::BeginPlay()
{
	Super::BeginPlay();

    EnableConstantRotation();
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
}


//------------------------------------------------------------
//------------------------------------------------------------
void AQLPickup::PlayAnimation(const FName& AnimationName)
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
void AQLPickup::EnableConstantRotation()
{
    GetWorldTimerManager().SetTimer(ConstantRotationTimerHandle,
        this,
        &AQLPickup::PerformConstantRotation,
        FApp::GetDeltaTime(), // time interval in second
        true, // loop
        0.0f); // delay in second
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPickup::DisableConstantRotation()
{
    GetWorldTimerManager().ClearTimer(ConstantRotationTimerHandle);
}


//------------------------------------------------------------
//------------------------------------------------------------
void AQLPickup::PerformConstantRotation()
{
    // derivation:
    // rotation rate [degree / sec] = angle per tick / time per tick
    // so angle per tick = rotation rate * time per tick

    float DeltaTime = FApp::GetDeltaTime();
    FRotator Increment(RotationRate.Pitch * DeltaTime, RotationRate.Yaw * DeltaTime, RotationRate.Roll * DeltaTime);
    FRotator Rotator = GetActorRotation();
    Rotator.Add(Increment.Pitch, Increment.Yaw, Increment.Roll);
    SetActorRotation(Rotator);
}

//------------------------------------------------------------
//------------------------------------------------------------
USphereComponent* AQLPickup::GetRootSphereComponent()
{
    return SphereComponent;
}