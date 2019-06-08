//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------


#include "QLPortalCompatibleActor.h"
#include "Components/BoxComponent.h"

//----------------------------------------
// Sets default values
//----------------------------------------
AQLPortalCompatibleActor::AQLPortalCompatibleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxComponent;
    BoxComponent->InitBoxExtent(FVector(100.0f));
    BoxComponent->SetSimulatePhysics(false);
    // should use ECollisionEnabled::QueryAndPhysics instead of ECollisionEnabled::QueryOnly
    // even though SetSimulatePhysics(false)
    // otherwise this actor will become transparent to gravity gun compatible actor
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    //BoxComponent->SetWorldScale3D(FVector(0.2f, 1.0f, 1.0f));

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);
    //float zDim = StaticMeshComponent->Bounds.BoxExtent.Z; // note: extent refers to half of the side
    //StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -2.0f * zDim));
    //StaticMeshComponent->SetWorldScale3D(FVector(0.4f, 2.0f, 2.0f));
    //StaticMeshComponent->bCastDynamicShadow = false; // avoid performance hit when the actor has large volume
}

//----------------------------------------
// Called when the game starts or when spawned
//----------------------------------------
void AQLPortalCompatibleActor::BeginPlay()
{
	Super::BeginPlay();

}

//----------------------------------------
// Called every frame
//----------------------------------------
void AQLPortalCompatibleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

