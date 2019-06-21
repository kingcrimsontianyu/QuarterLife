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
#include "QLPowerupProtection.generated.h"

class UPrimitiveComponent;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLPowerupProtection : public AQLPowerup
{
	GENERATED_BODY()

public:
    AQLPowerupProtection();

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
    virtual void UpdateProgressOnUMGInternal(const float Value) override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void UpdateProgressOnUMG() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void SetUMGVisibility(const bool bFlag) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float ProtectionMultiplier;
};
