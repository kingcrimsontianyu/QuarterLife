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
#include "QLPickup.h"
#include "QLCharacter.h"
#include "QLPowerup.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLPowerup : public AQLPickup
{
	GENERATED_BODY()

public:
    AQLPowerup();

protected:
    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float EffectDuration;

    FTimerHandle EffectTimerHandle;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY()
    TWeakObjectPtr<AQLCharacter> Beneficiary;
};
