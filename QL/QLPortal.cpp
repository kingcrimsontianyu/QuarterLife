//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------


#include "QLPortal.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"

//----------------------------------------
// Sets default values
//----------------------------------------
AQLPortal::AQLPortal() :
Spouse(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    BoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
    BoxComponent->SetSimulatePhysics(false);
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    FrameStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameStaticMesh"));
    FrameStaticMesh->SetupAttachment(RootComponent);

    DisplayPlaneStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayPlaneStaticMesh"));
    DisplayPlaneStaticMesh->SetupAttachment(RootComponent);

    SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
    SceneCaptureComponent->SetRelativeLocation(FVector(200.0f, 0.0f, 0.0f));
    SceneCaptureComponent->bEnableClipPlane = true;
    SceneCaptureComponent->bCaptureEveryFrame = true;
    SceneCaptureComponent->TextureTarget = nullptr;
    SceneCaptureComponent->SetupAttachment(RootComponent);

    RenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTarget"));
    RenderTarget->InitAutoFormat(1920, 1080);
    RenderTarget->AddressX = TextureAddress::TA_Wrap;
    RenderTarget->AddressY = TextureAddress::TA_Wrap;
}

//----------------------------------------
// Called when the game starts or when spawned
//----------------------------------------
void AQLPortal::BeginPlay()
{
	Super::BeginPlay();

}

//----------------------------------------
// Called every frame
//----------------------------------------
void AQLPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    UpdateSCC();
}

//----------------------------------------
//----------------------------------------
void AQLPortal::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    SceneCaptureComponent->TextureTarget = RenderTarget;

    UMaterialInterface* PortalMaterial = DisplayPlaneStaticMesh->GetMaterial(0);
    DynamicDisplayPlaneMaterial = DisplayPlaneStaticMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, PortalMaterial);
    DynamicDisplayPlaneMaterial->SetTextureParameterValue("PortalTexture", RenderTarget);
}

//----------------------------------------
//----------------------------------------
void AQLPortal::UpdateSCC()
{
    if (Spouse == nullptr)
    {
        return;
    }

    // update camera location and rotation
    APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

    FVector CameraLocation = cm->GetCameraLocation();
    CameraLocation = ConvertLocationToSpouseSpace(CameraLocation);
    SceneCaptureComponent->SetWorldLocation(CameraLocation);

    FRotator CameraRotation = cm->GetCameraRotation();
    CameraRotation = ConvertRotationToSpouseSpace(CameraRotation);
    SceneCaptureComponent->SetWorldRotation(CameraRotation);

    // update clip plane
    SceneCaptureComponent->ClipPlaneBase = Spouse->GetActorLocation();
    SceneCaptureComponent->ClipPlaneNormal = Spouse->GetActorForwardVector();
}

//----------------------------------------
//----------------------------------------
void AQLPortal::SetSpouse(AQLPortal* SpouseExt)
{
    if (SpouseExt != this)
    {
        Spouse = SpouseExt;
    }
}

//----------------------------------------
//----------------------------------------
FVector AQLPortal::ConvertDirectionToSpouseSpace(const FVector& OldDirection)
{
    if (Spouse == nullptr)
    {
        return OldDirection;
    }

    FRotator ThisPortalRotation = this->GetActorRotation();
    FRotator ThisPortalRotationInverse = this->GetActorRotation().GetInverse();
    FRotator SpousePortalRotation = Spouse->GetActorRotation();

    FVector temp = ThisPortalRotationInverse.RotateVector(OldDirection);
    temp.X = -temp.X;
    temp.Y = -temp.Y;

    FVector NewDirection = SpousePortalRotation.RotateVector(temp);

    return NewDirection;
}

//----------------------------------------
//----------------------------------------
FVector AQLPortal::ConvertLocationToSpouseSpace(const FVector& OldLocation)
{
    if (Spouse == nullptr)
    {
        return OldLocation;
    }

    FVector LocationDiff = this->GetActorLocation() - OldLocation;
    FVector temp = ConvertDirectionToSpouseSpace(LocationDiff);
    FVector NewLocation = Spouse->GetActorLocation() - temp;

    return NewLocation;
}

//----------------------------------------
//----------------------------------------
FRotator AQLPortal::ConvertRotationToSpouseSpace(const FRotator& OldRotator)
{
    if (Spouse == nullptr)
    {
        return OldRotator;
    }

    FRotationMatrix RotationMatrix(OldRotator);
    FVector X;
    FVector Y;
    FVector Z;
    RotationMatrix.GetScaledAxes(X, Y, Z);

    X = ConvertDirectionToSpouseSpace(X);
    Z = ConvertDirectionToSpouseSpace(Z);

    FMatrix result = FRotationMatrix::MakeFromXZ(X, Z);
    FRotator NewRotator = result.Rotator();

    return NewRotator;
}
