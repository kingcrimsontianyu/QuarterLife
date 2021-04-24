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
#include "QLAbilityEnum.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UENUM(BlueprintType)
enum class EQLAbility : uint8
{
    None UMETA(DisplayName = "None"),
    GhostWalk UMETA(DisplayName = "GhostWalk"),
    HealingRain UMETA(DisplayName = "HealingRain"),
    PiercingSight UMETA(DisplayName = "PiercingSight"),
    TheWorld UMETA(DisplayName = "TheWorld"),
    TimeTravel UMETA(DisplayName = "TimeTravel"),
};
