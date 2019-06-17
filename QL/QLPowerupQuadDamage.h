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

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void Reactivate();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void Deactivate();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void OnEffectGone();

protected:
    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY()
    TWeakObjectPtr<AQLCharacter> Beneficiary;
};
