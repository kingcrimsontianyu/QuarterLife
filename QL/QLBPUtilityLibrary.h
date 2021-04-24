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
#include "QLAbilityEnum.h"
#include "QLWeaponEnum.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "QLBPUtilityLibrary.generated.h"

class AExponentialHeightFog;
//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API UQLBPUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

    UFUNCTION(BlueprintCallable, Category = "C++")
    static void InitializePlayer(const EQLAbility InitialAbility,
        const EQLWeapon InitialWeapon,
        const UObject* WorldContextObj,
        const AActor* NearActor = nullptr,
        const AActor* FarActor = nullptr);

    //------------------------------------------------------------
    // To be used in a special scenario where the level has a sky atmosphere and an exponential height fog.
    // The fog actor assumes a flat world, and therefore botches the rendering when the player is far from
    // the earth into the space.
    // This function makes fog density gradually reduce to 0 when the player is leaving the earth,
    // or increase to the initial valu;e when the player is approaching the earth.
    // This function shall be called by the level's Tick() function.
    // The level shall have a variable to stash the initial value of the fog density,
    // which is InitialFogDensity.
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++")
    static void HandleExponentialHeightFogPerTick(const UObject* WorldContextObj,
        const float InitialFogDensity,
        AExponentialHeightFog* FogActor,
        const float InterpStartDistance,
        const float InterpEndDistance);
};
