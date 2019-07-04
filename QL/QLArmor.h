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
#include "QLArmor.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLArmor : public AQLPickup
{
	GENERATED_BODY()

public:
    AQLArmor();

protected:
    virtual void OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float ArmorIncrement;
};
