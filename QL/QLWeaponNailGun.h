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
#include "QLWeaponNailGun.generated.h"

class AQLNailProjectile;
//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLWeaponNailGun : public AQLWeapon
{
	GENERATED_BODY()

public:
    AQLWeaponNailGun();

    virtual void Tick(float DeltaTime) override;

    virtual void OnFire() override;

    virtual void OnFireRelease() override;

    virtual void OnFireHold() override;

    virtual void SpawnNailProjectile();

    virtual void StopFire() override;
protected:
    virtual void PostInitializeComponents() override;

    UPROPERTY(EditDefaultsOnly, Category = "C++Property")
    TSubclassOf<AQLNailProjectile> NailProjectileClass;
};
