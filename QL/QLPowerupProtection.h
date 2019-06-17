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

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void Reactivate();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void Deactivate();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void OnEffectEnd();

protected:
    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void UpdateProgressOnUMG() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float ProtectionMultiplier;
};
