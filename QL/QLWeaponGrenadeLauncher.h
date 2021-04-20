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
#include "QLWeaponGrenadeLauncher.generated.h"

class AQLRecyclerGrenadeProjectile;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLWeaponGrenadeLauncher : public AQLWeapon
{
	GENERATED_BODY()

public:
    AQLWeaponGrenadeLauncher();

    virtual void OnFire() override;

    virtual void OnFireRelease() override;

    virtual void SpawnProjectile() override;
protected:
    virtual void Tick(float DeltaTime) override;

    virtual void PostInitializeComponents() override;

    UPROPERTY(EditDefaultsOnly, Category = "C++Property")
    TSubclassOf<AQLRecyclerGrenadeProjectile> RecyclerGrenadeProjectileClass;
};
