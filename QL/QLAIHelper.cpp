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
#include "Components/BoxComponent.h"
#include "QLUtility.h"
#include "QLCharacterHelper.h"

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
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (int32 Idx = 0; Idx < NumBotsToSpawn; ++Idx)
    {
        FVector SpawnLocation = QLUtility::SamplePointFromDiskOnXYPlane(SpawnRadius, GetActorLocation());

        AQLCharacter* Bot = GetWorld()->SpawnActor<AQLCharacter>(CharacterClass, SpawnLocation, FRotator::ZeroRotator, SpawnParameters);
        Bot->SetIsBot(true);
        Bot->EquipAll();
        if (CharacterHelper.IsValid())
        {
            Bot->SetCharacterHelper(CharacterHelper.Get());
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAIHelper::SetCharacterHelper(AQLCharacterHelper* CharacterHelperExt)
{
    CharacterHelper = CharacterHelperExt;
}

