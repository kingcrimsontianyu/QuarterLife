//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLColoredPortal.h"
#include "Materials/Material.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "QLUtility.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLColoredPortal::AQLColoredPortal() :
PortalColor(EPortalColor::Invalid)
{
    BoxComponent->InitBoxExtent(FVector(50.0f, 120.0f, 150.0f));

    DisplayPlaneStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayPlaneStaticMesh"));
    DisplayPlaneStaticMesh->SetupAttachment(RootComponent);
    DisplayPlaneStaticMesh->SetSimulatePhysics(false);
    DisplayPlaneStaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

    // animation
    EnlargeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("EnlargeTimeline"));
    EnlargeTimelineInterpFunction.BindUFunction(this, FName(TEXT("EnlargeCallback")));
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLColoredPortal::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (PortalMaterialInstanceDynamic.IsValid())
    {
        PortalMaterialInstanceDynamic->SetScalarParameterValue("PortalScaleFactor", 0.1f);
    }

    if (EnlargeTimeline && EnlargeCurve)
    {
        EnlargeTimeline->AddInterpFloat(EnlargeCurve, EnlargeTimelineInterpFunction, FName(TEXT("Enlarge")));
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLColoredPortal::BeginPlay()
{
    // specify PortalMaterialInstanceDynamic
    UMaterialInterface* PortalMaterial = DisplayPlaneStaticMesh->GetMaterial(0);
    if (PortalMaterial)
    {
        UMaterialInstanceDynamic* Result = DisplayPlaneStaticMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, PortalMaterial);
        SetPortalMaterialInstanceDynamic(Result);
    }


    // call Blueprint "event begin play" function
    // then associate PortalMaterialInstanceDynamic with render target
    Super::BeginPlay();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLColoredPortal::Initialize(EPortalColor PortalColorExt, AQLPortal* SpouseExt)
{
    PortalColor = PortalColorExt;

    if (PortalColor == EPortalColor::Blue)
    {
        SetBlue();
    }
    else
    {
        SetOrange();
    }

    // tell myself that I have a wife (may be valid or nullptr)
    SetSpouse(SpouseExt);

    auto* ThisSpouse = Cast<AQLColoredPortal>(SpouseExt);

    // if my wife exists, tell her that she has a husband
    if (ThisSpouse)
    {
        ThisSpouse->SetSpouse(this);
        ThisSpouse->SetActive();
        SetActive();
    }
    else
    {
        SetInactive();
    }

    // animation
    if (EnlargeTimeline && EnlargeCurve)
    {
        EnlargeTimeline->PlayFromStart();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLColoredPortal::CleanUp()
{
    SetSpouse(nullptr);
    SetInactive();

    if (Spouse.IsValid())
    {
        AQLColoredPortal* ThisSpouse = Cast<AQLColoredPortal>(Spouse.Get());

        if (ThisSpouse)
        {
            ThisSpouse->SetSpouse(nullptr);
            ThisSpouse->SetInactive();
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLColoredPortal::SetBlue()
{
    if (PortalMaterialInstanceDynamic.IsValid())
    {
        PortalMaterialInstanceDynamic->SetScalarParameterValue("PortalColor", 0);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLColoredPortal::SetOrange()
{
    if (PortalMaterialInstanceDynamic.IsValid())
    {
        PortalMaterialInstanceDynamic->SetScalarParameterValue("PortalColor", 1);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLColoredPortal::SetActive()
{
    if (PortalMaterialInstanceDynamic.IsValid())
    {
        PortalMaterialInstanceDynamic->SetScalarParameterValue("PortalState", 1);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLColoredPortal::SetInactive()
{
    if (PortalMaterialInstanceDynamic.IsValid())
    {
        PortalMaterialInstanceDynamic->SetScalarParameterValue("PortalState", 0);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLColoredPortal::EnlargeCallback(float Val)
{
    if (PortalMaterialInstanceDynamic.IsValid())
    {
        PortalMaterialInstanceDynamic->SetScalarParameterValue("PortalScaleFactor", Val);
    }
}