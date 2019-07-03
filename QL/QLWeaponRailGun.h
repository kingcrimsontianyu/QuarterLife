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
#include "QLWeapon.h"
#include "QLRailBeam.h"
#include "Components/TimelineComponent.h"
#include "QLWeaponRailGun.generated.h"

class UCameraComponent;

//------------------------------------------------------------
// In Blueprint, set these properties
// - crosshair texture list
// - fire sound list
// - fire animation list
// - zoom curve
// - rail beam class
//
// Do not add beam component, because rail gun dynamically creates
// emitters in the world when fired, instead of using beam component.
//------------------------------------------------------------
UCLASS()
class QL_API AQLWeaponRailGun : public AQLWeapon
{
	GENERATED_BODY()

public:
    AQLWeaponRailGun();

    virtual void OnFire() override;

    virtual void OnAltFire() override;

    virtual void OnAltFireRelease();

    virtual void OnAltFireHold();

    virtual void PrepareForImpendingWeaponSwitch() override;

    virtual void SetDamageMultiplier(const float Value) override;
protected:
    virtual void Tick(float DeltaTime) override;

    virtual void PostInitializeComponents() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float ZoomDamage;

    UPROPERTY(EditDefaultsOnly, Category = "C++Property")
    TSubclassOf<AQLRailBeam> RailBeamClass;

    UPROPERTY()
    UTimelineComponent* ZoomTimeline;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++Property")
    UCurveFloat* ZoomCurve;

    FOnTimelineFloat ZoomTimelineInterpFunction;

    UFUNCTION()
    void ZoomCallback(float Val);

    UPROPERTY()
    bool bZoomedIn;

    UPROPERTY()
    float CurrentDamage;

    UPROPERTY()
    float FOVCached;

    UPROPERTY()
    UCameraComponent* CameraComponentCached;

    UPROPERTY()
    float ZoomDamageAdjusted;
};
