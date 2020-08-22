//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLJumpPad.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "QLUtility.h"
#include "Kismet/GameplayStatics.h"
#include "QLCharacter.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLJumpPad::AQLJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootBoxComponent"));
    RootBoxComponent->InitBoxExtent(FVector(150.0f, 150.0f, 100.0f));
    RootBoxComponent->SetSimulatePhysics(false);
    RootBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    RootBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    RootComponent = RootBoxComponent;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);
    StaticMeshComponent->SetSimulatePhysics(false);
    StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

    SoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundComponent"));
    SoundComponent->SetupAttachment(RootComponent);

    LaunchCharacterVelocity = FVector(0.0f, 0.0f, 2000.0f);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLJumpPad::BeginPlay()
{
	Super::BeginPlay();

}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLJumpPad::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (RootBoxComponent)
    {
        RootBoxComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AQLJumpPad::OnComponentBeginOverlapImpl);
        RootBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AQLJumpPad::OnComponentBeginOverlapImpl);
    }

    if (SoundComponent && SoundAttenuation)
    {
        SoundComponent->AttenuationSettings = SoundAttenuation;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLJumpPad::OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AQLCharacter* QLCharacter = Cast<AQLCharacter>(OtherActor);
    if (QLCharacter)
    {
        PlaySoundFireAndForget("BoinBounce");

        QLCharacter->LaunchCharacter(LaunchCharacterVelocity,
            false, // do not override X/Y component
            true); // override Z component
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLJumpPad::PlaySoundFireAndForget(const FName& SoundName, EVirtualizationMode VirtualizationMode)
{
    USoundBase** Result = SoundList.Find(SoundName);
    if (Result)
    {
        USoundBase* Sound = *Result;

        if (Sound && SoundAttenuation)
        {
            // force sound to be always played when EVirtualizationMode::PlayWhenSilent
            Sound->VirtualizationMode = VirtualizationMode;

            UGameplayStatics::PlaySoundAtLocation(GetWorld(),
                Sound,
                GetActorLocation(),
                GetActorRotation(),
                1.0f, // VolumeMultiplier
                1.0f, // PitchMultiplier
                0.0f, // StartTime
                SoundAttenuation);
        }
    }
}

