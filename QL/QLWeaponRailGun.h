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
#include "QLWeaponRailGun.generated.h"

//------------------------------------------------------------
// In Blueprint, set these properties
// - crosshair texture list
// - fire sound list
// - fire animation list
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

    virtual void InflictDamage();
protected:
    virtual void PostInitializeComponents() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float BasicDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float ScopeDamage;

    UPROPERTY(EditDefaultsOnly, Category = "C++Property")
    TSubclassOf<AQLRailBeam> RailBeamClass;
};
