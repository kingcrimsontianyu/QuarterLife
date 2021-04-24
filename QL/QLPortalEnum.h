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
#include "QLPortalEnum.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UENUM(BlueprintType)
enum class EPortalColor : uint8
{
    Invalid UMETA(DisplayName="Invalid"),
    Blue UMETA(DisplayName = "Blue"),
    Orange UMETA(DisplayName = "Orange"),
};
