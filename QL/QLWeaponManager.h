//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "QLWeapon.h"
#include "QLCharacter.h"
#include "QLWeaponManager.generated.h"

//----------------------------------------
//----------------------------------------
UCLASS(Blueprintable)
class QL_API UQLWeaponManager : public UObject
{
    GENERATED_BODY()

protected:
    class WeaponWrapper
    {
        AQLWeapon* Weapon;
    };

public:
    UQLWeaponManager();

    void SetUser(AQLCharacter* Character);

protected:
    AQLCharacter* User;

};
