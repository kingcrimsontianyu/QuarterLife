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

//------------------------------------------------------------
//------------------------------------------------------------
AQLAbilityPiercingSight::AQLAbilityPiercingSight()
{
    QLName = FName("PiercingSight");
    PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
    PostProcessComponent->bEnabled = false;

    // animation
    ScanEffectTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ScanEffectTimeline"));
    ScanEffectTimelineInterpFunction.BindUFunction(this, FName(TEXT("ScanEffectCallback")));

    ScanSpeed = 5000.0f;
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
        if (MaterialPiercingSight)
        {
            FWeightedBlendable WeightedBlendable;
            DynamicMaterialPiercingSight = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, MaterialPiercingSight);
            WeightedBlendable.Object = DynamicMaterialPiercingSight.Get();
            WeightedBlendable.Weight = 1.0f;
            PostProcessComponent->Settings.WeightedBlendables.Array.Add(WeightedBlendable);
        }
    }

    if (ScanEffectTimeline && ScanEffectCurve)
    {
        ScanEffectTimeline->AddInterpFloat(ScanEffectCurve, ScanEffectTimelineInterpFunction, FName(TEXT("ScanEffect")));
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityPiercingSight::OnUse()
{
    PostProcessComponent->bEnabled = true;

    if (ScanEffectTimeline && ScanEffectCurve)
    {
        ScanEffectTimeline->PlayFromStart();

        // once the effect starts, periodically update
        GetWorldTimerManager().SetTimer(ScanEffectEndTimerHandle,
            this,
            &AQLAbilityPiercingSight::OnScanEffectEnd,
            0.1f, // time interval in second
            false, // loop
            ScanEffectTimeline->GetTimelineLength()); // delay in second
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityPiercingSight::ScanEffectCallback(float Val)
{
    if (DynamicMaterialPiercingSight.IsValid())
    {
        float InnerRadius = 200.0f + ScanSpeed * Val;
        DynamicMaterialPiercingSight->SetScalarParameterValue("InnerRadius", InnerRadius);
        DynamicMaterialPiercingSight->SetScalarParameterValue("OuterRadius", InnerRadius + 50.0f);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityPiercingSight::OnScanEffectEnd()
{
    PostProcessComponent->bEnabled = false;
}