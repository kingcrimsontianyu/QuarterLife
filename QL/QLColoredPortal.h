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
#include "QLPortal.h"
#include "QLPortalEnum.h"
#include "Components/TimelineComponent.h"
#include "QLColoredPortal.generated.h"

//------------------------------------------------------------
// Blue / orange portal used for the portal gun
//------------------------------------------------------------
UCLASS()
class QL_API AQLColoredPortal : public AQLPortal
{
	GENERATED_BODY()

public:
    AQLColoredPortal();

    //------------------------------------------------------------
    // Given the type of portal and its spouse,
    // initialize self and spouse
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void Initialize(EPortalColor PortalColor, AQLPortal* SpouseExt);

    //------------------------------------------------------------
    // Clean up self and spouse
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void CleanUp();

    //------------------------------------------------------------
    // Set the portal color to blue
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetBlue();

    //------------------------------------------------------------
    // Set the portal color to orange
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetOrange();

    //------------------------------------------------------------
    // Set the portal state to active where the
    // portal appears as a circular band
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetActive();

    //------------------------------------------------------------
    // Set the portal state to inactive where the
    // portal appears as a filled circle
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetInactive();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    EPortalColor PortalColor;

protected:
    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

    UTimelineComponent* EnlargeTimeline;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++Property")
    UCurveFloat* EnlargeCurve;

    FOnTimelineFloat EnlargeTimelineInterpFunction;

    UFUNCTION()
    void EnlargeCallback(float Val);
};
