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
#include "UObject/NoExportTypes.h"
#include "QLPowerupManager.generated.h"

class AQLCharacter;
class AQLPowerup;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API UQLPowerupManager : public UObject
{
    GENERATED_BODY()

public:
    UQLPowerupManager();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetUser(AQLCharacter* Character);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    AQLCharacter* GetUser();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    bool AddPowerup(AQLPowerup* Powerup);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void RemovePowerup(AQLPowerup* Powerup);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    AQLPowerup* GetTopPowerup();
protected:
    // do not use UPROPERTY() here
    // it breaks the character weapon system
    // to do: need to understand why
    TWeakObjectPtr<AQLCharacter> User;

    UPROPERTY()
    TArray<AQLPowerup*> PowerupList;
};
