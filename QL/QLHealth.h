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
    virtual void OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float HealthIncrement;
};
