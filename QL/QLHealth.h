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
#include "QLHealth.generated.h"

class AQLCharacter;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLHealth : public AQLPickup
{
	GENERATED_BODY()

public:
    AQLHealth();

protected:
    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void AddHealthToPlayer(AQLCharacter* QLCharacter);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float HealthIncrement;
};
