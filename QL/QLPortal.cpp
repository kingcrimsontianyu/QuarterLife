//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLPortal.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "QLUtility.h"
#include "Engine/Engine.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLPortal::AQLPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    BoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
    BoxComponent->SetSimulatePhysics(false);
    BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
    SceneCaptureComponent->SetRelativeLocation(FVector(200.0f, 0.0f, 0.0f));
    SceneCaptureComponent->bEnableClipPlane = true;
    SceneCaptureComponent->bCaptureEveryFrame = true;
    SceneCaptureComponent->TextureTarget = nullptr;
    SceneCaptureComponent->SetupAttachment(RootComponent);

    bCanUpdatePortalView = true;

    bCanTeleport = true;
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLPortal::BeginPlay()
{
    // call Blueprint "event begin play" function
    Super::BeginPlay();

    // Blueprint subclass of AQLPortal should call SetPortalMaterialInstanceDynamic() to
    // initialize PortalMaterialInstanceDynamic
    if (PortalMaterialInstanceDynamic.IsValid())
    {
        PortalMaterialInstanceDynamic->SetTextureParameterValue("PortalTexture", RenderTarget);
    }
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bCanUpdatePortalView)
    {
        UpdateSCC();
    }
}

//------------------------------------------------------------
// To list the available material parameters, use:
// TArray<FMaterialParameterInfo> outParamInfo;
// TArray<FGuid> outParamIds;
// PortalMaterialInstanceDynamic->GetAllTextureParameterInfo(outParamInfo, outParamIds);
// for (auto&& item : outParamInfo)
// {
//     QLUtility::Screen(item.Name.ToString());
// }
//------------------------------------------------------------
void AQLPortal::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    RenderTarget = NewObject<UTextureRenderTarget2D>(this);

    // set up scene capture component and render target
    if (SceneCaptureComponent && RenderTarget)
    {
        // todo: (1) how to avoid hardcoded resolution?!
        //       (2) how to conduct antialiasing for the render target?!

        uint32 ResolutionX = 1920 / 2;
        uint32 ResolutionY = 1080 / 2;
        RenderTarget->InitAutoFormat(ResolutionX, ResolutionY);
        RenderTarget->AddressX = TextureAddress::TA_Wrap;
        RenderTarget->AddressY = TextureAddress::TA_Wrap;

        SceneCaptureComponent->bEnableClipPlane = true;
        SceneCaptureComponent->TextureTarget = RenderTarget;

        // set up to manually update scene capture instead of automatically
        // when set to false, the component will render once on load and then only when moved
        SceneCaptureComponent->bCaptureEveryFrame = false;
        // when set to false, the capture component's content will not be automatically updated upon movement
        SceneCaptureComponent->bCaptureOnMovement = false;

        SceneCaptureComponent->bEnableClipPlane = true;

        // must be identical to player camera's FOV
        SceneCaptureComponent->FOVAngle = 100.0f;

        SceneCaptureComponent->ProjectionType = ECameraProjectionMode::Perspective;
    }

    // built-in dynamic delegate
    OnActorBeginOverlap.AddDynamic(this, &AQLPortal::OnOverlapBeginForActor);
    OnActorEndOverlap.AddDynamic(this, &AQLPortal::OnOverlapEndForActor);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::UpdateSCC()
{
    if (!Spouse.IsValid())
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

    // manually capture the scene after spatial transform is completed
    SceneCaptureComponent->CaptureScene();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::SetSpouse(AQLPortal* SpouseExt)
{
    if (SpouseExt != this)
    {
        Spouse = SpouseExt;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
FVector AQLPortal::ConvertDirectionToSpouseSpace(const FVector& OldDirection)
{
    if (!Spouse.IsValid())
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

//------------------------------------------------------------
//------------------------------------------------------------
FVector AQLPortal::ConvertLocationToSpouseSpace(const FVector& OldLocation)
{
    if (!Spouse.IsValid())
    {
        return OldLocation;
    }

    FVector LocationDiff = this->GetActorLocation() - OldLocation;
    FVector temp = ConvertDirectionToSpouseSpace(LocationDiff);
    FVector NewLocation = Spouse->GetActorLocation() - temp;

    return NewLocation;
}

//------------------------------------------------------------
//------------------------------------------------------------
FRotator AQLPortal::ConvertRotationToSpouseSpace(const FRotator& OldRotator)
{
    if (!Spouse.IsValid())
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

//------------------------------------------------------------
//------------------------------------------------------------
UBoxComponent* AQLPortal::GetBoxComponent()
{
    return BoxComponent;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::Debug()
{
    QLUtility::Log(SceneCaptureComponent->GetComponentLocation().ToString());
    QLUtility::Log(SceneCaptureComponent->GetComponentRotation().ToString());
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::SetCanUpdatePortalView(bool bFlag)
{
    bCanUpdatePortalView = bFlag;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor)
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::OnOverlapEndForActor(AActor* OverlappedActor, AActor* OtherActor)
{
    // remove actor from my own roll if any
    RemoveFromRoll(OtherActor);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::AddToRoll(AActor* GivenActor)
{
    // if key exists, overwrite the value
    Roll.Add(GivenActor);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::RemoveFromRoll(AActor* GivenActor)
{
    // remove all the elements that match the given parameter
    Roll.Remove(GivenActor);
}

//------------------------------------------------------------
//------------------------------------------------------------
bool AQLPortal::IsInMyRoll(AActor* GivenActor)
{
    return Roll.Contains(GivenActor);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::SetPortalMaterialInstanceDynamic(UMaterialInstanceDynamic* PortalMaterialInstanceDynamicExt)
{
    PortalMaterialInstanceDynamic = PortalMaterialInstanceDynamicExt;
}