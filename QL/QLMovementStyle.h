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
#include "QLMovementStyle.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UENUM()
enum class EQLMovementStyle : uint8
{
    Default,
    QuakeVallina,
    QuakeCPMA, // Challenge ProMode Arena
};
