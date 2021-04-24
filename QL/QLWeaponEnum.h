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
#include "QLWeaponEnum.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UENUM(BlueprintType)
enum class EQLWeapon : uint8
{
    None UMETA(DisplayName = "None"),
    Gauntlet UMETA(DisplayName = "Gauntlet"),
    RocketLauncher UMETA(DisplayName = "RocketLauncher"),
    GrenadeLauncher UMETA(DisplayName = "GrenadeLauncher"),
    LightningGun UMETA(DisplayName = "LightningGun"),
    NailGun UMETA(DisplayName = "NailGun"),
    PortalGun UMETA(DisplayName = "PortalGun"),
    RailGun UMETA(DisplayName = "RailGun"),
};
