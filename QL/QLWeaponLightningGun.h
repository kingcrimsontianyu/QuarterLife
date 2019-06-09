//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "QLWeapon.h"
#include "QLWeaponLightningGun.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLWeaponLightningGun : public AQLWeapon
{
	GENERATED_BODY()

public:
    AQLWeaponLightningGun();

    virtual void Tick(float DeltaTime) override;

    virtual void Fire() override;

    virtual void FireRelease() override;

    void DealDamageIfHit();

    bool HasHitEnemy();

protected:
    virtual void PostInitializeComponents() override;


};
