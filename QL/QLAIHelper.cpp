//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLAIHelper.h"
#include "QLCharacter.h"
#include "Components/SphereComponent.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLAIHelper::AQLAIHelper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpawnRadius = 1000.0f;
    NumBotsToSpawn = 3;

    RootSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphereComponent"));
    RootSphereComponent->InitSphereRadius(SpawnRadius);
    RootSphereComponent->SetSimulatePhysics(false);
    RootSphereComponent->SetCollisionProfileName(FName(TEXT("NoCollision")));
    RootComponent = RootSphereComponent;

    CharacterClass = AQLCharacter::StaticClass();
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLAIHelper::BeginPlay()
{
	Super::BeginPlay();

    SpawnBots();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAIHelper::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    RootSphereComponent->InitSphereRadius(SpawnRadius);
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLAIHelper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAIHelper::SpawnBots()
{
    constexpr float QLTwoPi = 2.0f * PI;
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (int32 Idx = 0; Idx < NumBotsToSpawn; ++Idx)
    {
        // sample a point in a circle
        // reference:
        // - http://mathworld.wolfram.com/DiskPointPicking.html
        // - https://www.arl.army.mil/arlreports/2015/ARL-TR-7333.pdf
        float RandomTheta = FMath::RandRange(0.0f, QLTwoPi);
        float RandomU = FMath::RandRange(0.0f, SpawnRadius * SpawnRadius);

        float Radius = FMath::Sqrt(RandomU);
        float x = GetActorLocation().X + Radius * FMath::Cos(RandomTheta);
        float y = GetActorLocation().Y + Radius * FMath::Sin(RandomTheta);

        FVector SpawnLocation(x, y, GetActorLocation().Z);

        GetWorld()->SpawnActor<AQLCharacter>(CharacterClass, SpawnLocation, FRotator::ZeroRotator, SpawnParameters);
    }
}

