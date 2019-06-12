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
#include "ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "QLUtility.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLColoredPortal::AQLColoredPortal() :
PortalColor(EPortalColor::Invalid),
EnlargeCurve(nullptr)
{
    BoxComponent->InitBoxExtent(FVector(50.0f, 120.0f, 150.0f));

    // animation
    EnlargeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("EnlargeTimeline"));
    EnlargeTimelineInterpFunction.BindUFunction(this, FName{ TEXT("EnlargeCallback") });
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLColoredPortal::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (DynamicDisplayPlaneMaterial)
    {
        DynamicDisplayPlaneMaterial->SetScalarParameterValue("PortalScaleFactor", 0.1);
    }

    if (EnlargeTimeline && EnlargeCurve)
    {
        EnlargeTimeline->AddInterpFloat(EnlargeCurve, EnlargeTimelineInterpFunction, FName(TEXT("Enlarge")));
    }
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

    auto* ThisSpouse = Cast<AQLColoredPortal>(Spouse);

    if (ThisSpouse)
    {
        ThisSpouse->SetSpouse(nullptr);
        ThisSpouse->SetInactive();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLColoredPortal::SetBlue()
{
    DynamicDisplayPlaneMaterial->SetScalarParameterValue("PortalColor", 0);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLColoredPortal::SetOrange()
{
    DynamicDisplayPlaneMaterial->SetScalarParameterValue("PortalColor", 1);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLColoredPortal::SetActive()
{
    DynamicDisplayPlaneMaterial->SetScalarParameterValue("PortalState", 1);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLColoredPortal::SetInactive()
{
    DynamicDisplayPlaneMaterial->SetScalarParameterValue("PortalState", 0);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLColoredPortal::EnlargeCallback(float Val)
{
    if (DynamicDisplayPlaneMaterial)
    {
        DynamicDisplayPlaneMaterial->SetScalarParameterValue("PortalScaleFactor", Val);
    }
}