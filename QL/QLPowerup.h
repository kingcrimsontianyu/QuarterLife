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
#include "QLPowerup.generated.h"

class AQLCharacter;
class UQLPowerupManager;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLPowerup : public AQLPickup
{
	GENERATED_BODY()

public:
    AQLPowerup();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetProgressPercent();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetPowerupManager(UQLPowerupManager* PowerupManagerExt);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FName GetPowerupName();
protected:
    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void UpdateProgressOnUMG();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void PowerUpPlayer();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void Reactivate();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void Deactivate();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void OnEffectEnd();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float EffectDuration;

    FTimerHandle EffectStartTimerHandle;
    FTimerHandle EffectEndTimerHandle;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY()
    TWeakObjectPtr<AQLCharacter> Beneficiary;

    UPROPERTY()
    TWeakObjectPtr<UQLPowerupManager> PowerupManager;

    float ProgressPercent;
    float TimeElapsed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    FName PowerupName;
};
