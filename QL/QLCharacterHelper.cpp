//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLCharacterHelper.h"
#include "Components/BoxComponent.h"
#include "QLUtility.h"
#include "Kismet/GameplayStatics.h"
#include "QLGameModeBase.h"
#include "QLCharacter.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLCharacterHelper::AQLCharacterHelper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RespawnExtent = FVector(5000.0f, 5000.0f, 100.0f);
    RespawnBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootBoxComponent"));
    RespawnBoxComponent->InitBoxExtent(RespawnExtent);
    RespawnBoxComponent->SetSimulatePhysics(false);
    RespawnBoxComponent->SetCollisionProfileName(FName(TEXT("NoCollision")));
    RootComponent = RespawnBoxComponent;
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLCharacterHelper::BeginPlay()
{
	Super::BeginPlay();

}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacterHelper::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    RespawnBoxComponent->InitBoxExtent(RespawnExtent);
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLCharacterHelper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacterHelper::RespawnCharacterRandomly(bool bSpawnAsBot)
{
    FVector Extent = RespawnBoxComponent->GetScaledBoxExtent();

    FVector RandomLocation = QLUtility::SamplePointFromSquareOnXYPlane(Extent.X / 2.0f, Extent.Y / 2.0f, RespawnBoxComponent->GetComponentLocation());
    FRotator RandomYawRotation = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

    AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(GetWorld());

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    AQLCharacter* NewCharacter = GetWorld()->SpawnActor<AQLCharacter>(GameModeBase->DefaultPawnClass, RandomLocation, RandomYawRotation, SpawnParameters);

    if (bSpawnAsBot)
    {
        NewCharacter->SetIsBot(true);
    }
    else
    {
        NewCharacter->SetIsBot(false);
        UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(NewCharacter);
    }

    NewCharacter->EquipAll();
    NewCharacter->SetCharacterHelper(this);
}
