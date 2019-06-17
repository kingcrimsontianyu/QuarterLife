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
#include "QLRocketProjectile.h"
#include "QLWeaponRocketLauncher.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLWeaponRocketLauncher : public AQLWeapon
{
	GENERATED_BODY()

public:
    AQLWeaponRocketLauncher();

    virtual void OnFire() override;

    virtual void SetDamageMultiplier(const float Value) override;
protected:
    virtual void Tick(float DeltaTime) override;

    virtual void PostInitializeComponents() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float BasicDamage;

    UPROPERTY()
    float BasicDamageAdjusted;

    UPROPERTY(EditDefaultsOnly, Category = "C++Property")
    TSubclassOf<AQLRocketProjectile> RocketProjectileClass;
};
