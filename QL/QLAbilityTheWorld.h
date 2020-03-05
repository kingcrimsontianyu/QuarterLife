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
#include "QLAbilityTheWorld.generated.h"

class UPostProcessComponent;
//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLAbilityTheWorld : public AQLAbility
{
	GENERATED_BODY()

public:
    AQLAbilityTheWorld();

    virtual void OnUse() override;

    virtual void OnAbilityEnd() override;

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
    void TheWorldCallback(float Val);

    UFUNCTION()
    void OnTimeStopped();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++Property")
    UPostProcessComponent* PostProcessComponent;

    UPROPERTY()
    TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterialTheWorld;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    UMaterialInterface* MaterialTheWorld;

    UPROPERTY()
    UTimelineComponent* TheWorldTimeline;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++Property")
    UCurveFloat* TheWorldCurve;

    FOnTimelineFloat TheWorldTimelineInterpFunction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float TimeDilationFactor;

    FTimerHandle DelayTimerHandle;
};
