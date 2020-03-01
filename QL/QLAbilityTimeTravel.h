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
#include "QLAbilityTimeTravel.generated.h"

class UPostProcessComponent;
//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLAbilityTimeTravel : public AQLAbility
{
	GENERATED_BODY()

public:
    AQLAbilityTimeTravel();

    virtual void OnUse() override;

    virtual void OnAbilityEnd() override;

    void SetNearAndFarActors(AActor* NearActorExt, AActor* FarActorExt);

protected:
    //------------------------------------------------------------
    // Called when the game starts or when spawned
    //------------------------------------------------------------
    virtual void BeginPlay() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

    void SwapNearAndFarActor();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION()
    void TimeTravelCallback(float Val);

protected:
    UPROPERTY()
    TWeakObjectPtr<AActor> NearActor;

    UPROPERTY()
    TWeakObjectPtr<AActor> FarActor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++Property")
    UPostProcessComponent* PostProcessComponent;

    UPROPERTY()
    TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterialTimeTravel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    UMaterialInterface* MaterialTimeTravel;

    UPROPERTY()
    UTimelineComponent* TimeTravelTimeline;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++Property")
    UCurveFloat* TimeTravelCurve;

    FOnTimelineFloat TimeTravelTimelineInterpFunction;

    int SoundIdx;
};
