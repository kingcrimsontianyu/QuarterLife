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
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLAIHelper::AQLAIHelper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    NumBotsToSpawn = 3;

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
    for (int32 Idx = 0; Idx < NumBotsToSpawn; ++Idx)
    {
        UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

        if (NavSys)
        {
            FNavLocation RandomLocation;
            bool bFound = NavSys->GetRandomPoint(RandomLocation);
            if (!bFound)
            {
                return;
            }
            RandomLocation.Location.Z += 100.0f;

            FRotator RandomYawRotation = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);
            FTransform RandomTransform(RandomYawRotation, RandomLocation.Location, FVector(1.0f));

            // deferred spawn in order to timely specify human/bot identity
            AQLCharacter* Bot = GetWorld()->SpawnActorDeferred<AQLCharacter>(CharacterClass, RandomTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
            Bot->SetIsBot(true);
            UGameplayStatics::FinishSpawningActor(Bot, RandomTransform);

            Bot->EquipAll();
        }
    }
}

