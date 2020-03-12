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
#include "Engine/DataAsset.h"
#include "QLMovementDataQuake.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS(Blueprintable)
class QL_API UQLMovementDataQuake : public UObject
{
	GENERATED_BODY()

public:
    UQLMovementDataQuake();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float MaxWalkSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float AirControl;
};
