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
#include "QLPowerup.h"
#include "QLCharacter.h"
#include "QLPowerupQuadDamage.generated.h"

class UPrimitiveComponent;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLPowerupQuadDamage : public AQLPowerup
{
	GENERATED_BODY()

public:
    AQLPowerupQuadDamage();

protected:
    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PowerUpPlayer() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void OnEffectEnd() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void UpdateProgressOnUMG() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float DamageMultiplier;
};
