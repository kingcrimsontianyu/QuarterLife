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
class AQLPortal;
//------------------------------------------------------------
// AQLAbilityTimeTravel has several key components
// --- NearActor, FarActor
//     Reference actors (such as walls) on the map. NearActor is always closer to the player than FarActor.
//     If at some point NearActor becomes farther, it is swapped with FarActor.
// --- ShadowAbility
//     A pseudo actor that is the shadow of the AQLAbilityTimeTravel actor.
//     The relative transform between ShadowAbility and FarActor is always equal to that between AQLAbilityTimeTravel and NearActor.
// --- NearPortal
//     Actor to display the portal view. NearPortal is attached to the AQLAbilityTimeTravel actor with a fixed relative transform.
//     Has a skeletal mesh model resembling the timepiece in Dishonored 2
// --- FarPortal
//     FarPortal is attached to the ShadowAbility actor with a fixed relative transform.
//     Has a skeletal mesh model but is set to invisible
//------------------------------------------------------------
UCLASS()
class QL_API AQLAbilityTimeTravel : public AQLAbility
{
	GENERATED_BODY()

public:
    AQLAbilityTimeTravel();

    virtual void OnUse() override;

    virtual void OnAbilityEnd() override;

    virtual void OnAbilitySetCurrent() override;

    virtual void OnAbilityUnsetCurrent() override;

    void SetNearAndFarActors(AActor* NearActorExt, AActor* FarActorExt);

    virtual void Debug() override;

protected:
    //------------------------------------------------------------
    // Called when the game starts or when spawned
    //------------------------------------------------------------
    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void SwapNearAndFarActor();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION()
    void TimeTravelCallback(float Val);

    //------------------------------------------------------------
    //------------------------------------------------------------
    FTransform CalculateShadowCharacterTransform();

    //------------------------------------------------------------
    //------------------------------------------------------------
    FTransform CalculateShadowAbilityTransform();

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TSubclassOf<AQLPortal> PortalClass;

    UPROPERTY()
    AQLPortal* NearPortal;

    UPROPERTY()
    AQLPortal* FarPortal;

    // If AActor* is used, ShadowAbility cannot be transformed freely
    UPROPERTY()
    AQLPickup* ShadowAbility;
};
