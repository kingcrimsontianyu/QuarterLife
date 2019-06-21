//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "QLAbility.h"
#include "Components/TimelineComponent.h"
#include "QLAbilityPiercingSight.generated.h"

class UPostProcessComponent;
//------------------------------------------------------------
// In Blueprint, set these properties
// - material piercing sight
//------------------------------------------------------------
UCLASS()
class QL_API AQLAbilityPiercingSight : public AQLAbility
{
	GENERATED_BODY()

public:
    AQLAbilityPiercingSight();

    virtual void OnUse() override;
protected:
    //------------------------------------------------------------
    // Called when the game starts or when spawned
    //------------------------------------------------------------
    virtual void BeginPlay() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION()
    void OnScanEffectEnd();

    FOnTimelineEventStatic OnScanEffectEndCallback;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++Property")
    UPostProcessComponent* PostProcessComponent;

    UPROPERTY()
    TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterialScanEffect;

    UPROPERTY()
    TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterialXRayEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    UMaterialInterface* MaterialScanEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    UMaterialInterface* MaterialXRayEffect;

    UPROPERTY()
    UTimelineComponent* ScanEffectTimeline;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++Property")
    UCurveFloat* ScanEffectCurve;

    FOnTimelineFloat ScanEffectTimelineInterpFunction;

    UFUNCTION()
    void ScanEffectCallback(float Val);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float ScanSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    int32 ScanTimes;

    int32 Counter;
};
