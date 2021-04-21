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

    // these sound components will be called by animation BP to sync sound with animation, hence public
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UAudioComponent* SoundComponentEngineStart;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UAudioComponent* SoundComponentConstRot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UAudioComponent* SoundComponentEngineStop;
protected:
    virtual void Tick(float DeltaTime) override;

    virtual void PostInitializeComponents() override;

    UPROPERTY()
    UAnimMontage* FireMontage;
};
