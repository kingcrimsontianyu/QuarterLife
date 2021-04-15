//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLCelestialBody.h"
#include "Components/StaticMeshComponent.h"
#include "Math/UnrealMathUtility.h"
#include "QLUtility.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLCelestialBody::AQLCelestialBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bCanRotate = true;
    RotationRateInDegree = 30.0f;
    RotationAxis = FVector(0.0f, 0.0f, 1.0f);
    bCanRevolve = false;
    RevolutionRateInDegree = 30.0f;
    RevolutionAxisApproximate = FVector(0.0f, 0.0f, 1.0f);
    bCanCastShadow = false;
    BaseColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

    BasicStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameStaticMesh"));
    BasicStaticMesh->SetupAttachment(RootComponent);
    BasicStaticMesh->SetSimulatePhysics(false);
    BasicStaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCelestialBody::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    UMaterialInterface* BasicStaticMeshMaterial = BasicStaticMesh->GetMaterial(0);
    if (BasicStaticMeshMaterial)
    {
        DynamicBasicStaticMeshMaterial = BasicStaticMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BasicStaticMeshMaterial);
        if (DynamicBasicStaticMeshMaterial)
        {
            DynamicBasicStaticMeshMaterial->SetVectorParameterValue("BaseColor", BaseColor);
        }
    }

    BasicStaticMesh->CastShadow = bCanCastShadow;
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLCelestialBody::BeginPlay()
{
	Super::BeginPlay();

    if (bCanRevolve && RevolveAroundActor.IsValid())
    {
        FVector SunLocation(RevolveAroundActor->GetActorLocation());

        FVector TempLocation = GetActorLocation() - SunLocation;

        FVector V1(-TempLocation);
        V1.Normalize(); // in-place normalize

        RevolutionAxisNorm = RevolutionAxisApproximate - V1 * FVector::DotProduct(V1, RevolutionAxisApproximate);
        RevolutionAxisNorm.Normalize();
    }
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLCelestialBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bCanRotate)
    {
        // derivation:
        // rotation rate [degree / sec] = angle per tick / time per tick
        // so angle per tick = rotation rate * time per tick

        float IncrementAngle = FMath::DegreesToRadians(RotationRateInDegree * DeltaTime);

        FVector RotationAxisNorm(RotationAxis);
        RotationAxisNorm.Normalize();

        FQuat MyQuat(RotationAxisNorm, IncrementAngle);

        AddActorLocalRotation(MyQuat);
    }

    // we want to apply revolution to the center of this actor, not every point on the actor
    // as a result, only translation is applied to this actor, not rotation
    if (bCanRevolve && RevolveAroundActor.IsValid())
    {
        FVector SunLocation(RevolveAroundActor->GetActorLocation());

        FVector TempLocation = GetActorLocation() - SunLocation;

        float IncrementAngle = FMath::DegreesToRadians(RevolutionRateInDegree * DeltaTime);

        FQuat MyQuat(RevolutionAxisNorm, IncrementAngle);

        FVector MyLoc = MyQuat.RotateVector(TempLocation) + SunLocation;

        SetActorLocation(MyLoc);
    }

}

