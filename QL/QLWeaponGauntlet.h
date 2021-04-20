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
#include "QLWeaponGauntlet.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLWeaponGauntlet : public AQLWeapon
{
	GENERATED_BODY()

public:
    AQLWeaponGauntlet();

    virtual void OnFire() override;

    virtual void OnFireRelease() override;

    virtual void PerformDrill();
protected:
    virtual void Tick(float DeltaTime) override;

    virtual void PostInitializeComponents() override;
};
