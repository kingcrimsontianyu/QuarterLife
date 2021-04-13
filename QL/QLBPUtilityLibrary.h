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
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QLBPUtilityLibrary.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API UQLBPUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

    UFUNCTION(BlueprintCallable, Category = "C++")
    static void InitializePlayer(const FString& InitialAbility,
        const FString& InitialWeapon,
        const UObject* WorldContextObj,
        const AActor* NearActor = nullptr,
        const AActor* FarActor = nullptr);
};
