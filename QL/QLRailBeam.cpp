//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLRailBeam.h"
#include "Particles/ParticleSystemComponent.h"
#include "QLUtility.h"
#include "Components/SphereComponent.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLRailBeam::AQLRailBeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphereComponent"));

    BeamComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BeamComponent"));
    BeamComponent->SetupAttachment(RootComponent);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLRailBeam::BeginPlay()
{
	Super::BeginPlay();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLRailBeam::BeginDestroy()
{
    Super::BeginDestroy();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLRailBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//------------------------------------------------------------
//------------------------------------------------------------
UParticleSystemComponent* AQLRailBeam::GetBeamComponent()
{
    return BeamComponent;
}
