//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLAbilityPiercingSight.h"
#include "Components/PostProcessComponent.h"
#include "Components/SphereComponent.h"
#include "QLUtility.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "QLAbilityManager.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLAbilityPiercingSight::AQLAbilityPiercingSight()
{
    QLName = FName(TEXT("PiercingSight"));
    PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
    PostProcessComponent->bEnabled = false;

    // animation
    ScanEffectTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ScanEffectTimeline"));
    ScanEffectTimelineInterpFunction.BindUFunction(this, FName(TEXT("ScanEffectCallback")));
    OnScanEffectEndCallback.BindUFunction(this, FName(TEXT("OnScanEffectEnd")));

    ScanSpeed = 5000.0f;
    ScanTimes = 4;
    Counter = 0;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityPiercingSight::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityPiercingSight::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (PostProcessComponent)
    {
        if (MaterialScanEffect)
        {
            FWeightedBlendable WeightedBlendable;
            DynamicMaterialScanEffect = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, MaterialScanEffect);
            WeightedBlendable.Object = DynamicMaterialScanEffect.Get();
            WeightedBlendable.Weight = 1.0f;
            PostProcessComponent->Settings.WeightedBlendables.Array.Add(WeightedBlendable);
        }

        if (MaterialXRayEffect)
        {
            FWeightedBlendable WeightedBlendable;
            DynamicMaterialXRayEffect = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, MaterialXRayEffect);
            WeightedBlendable.Object = DynamicMaterialXRayEffect.Get();
            WeightedBlendable.Weight = 1.0f;
            PostProcessComponent->Settings.WeightedBlendables.Array.Add(WeightedBlendable);
        }
    }

    if (ScanEffectTimeline && ScanEffectCurve)
    {
        ScanEffectTimeline->AddInterpFloat(ScanEffectCurve, ScanEffectTimelineInterpFunction, FName(TEXT("ScanEffect")));
        ScanEffectTimeline->SetTimelineFinishedFunc(OnScanEffectEndCallback);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityPiercingSight::OnUse()
{
    Super::OnUse();

    if (!IsActive())
    {
        return;
    }

    Counter = 0;
    PostProcessComponent->bEnabled = true;
    PlaySound(FName(TEXT("Scan")));

    if (ScanEffectTimeline && ScanEffectCurve)
    {
        ScanEffectTimeline->PlayFromStart();
    }

    Deactivate();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityPiercingSight::ScanEffectCallback(float Val)
{
    if (DynamicMaterialScanEffect.IsValid())
    {
        float InnerRadius = 200.0f + ScanSpeed * Val;
        DynamicMaterialScanEffect->SetScalarParameterValue("ScanEffectInnerRadius", InnerRadius);
        DynamicMaterialScanEffect->SetScalarParameterValue("ScanEffectOuterRadius", InnerRadius + 50.0f);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityPiercingSight::OnScanEffectEnd()
{
    ++Counter;

    if (Counter >= ScanTimes)
    {
        PostProcessComponent->bEnabled = false;
    }
    else
    {
        ScanEffectTimeline->PlayFromStart();
    }
}
