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
#include "QLWeaponManager.generated.h"

class AQLCharacter;
class AQLWeapon;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS(Blueprintable)
class QL_API UQLWeaponManager : public UObject
{
    GENERATED_BODY()

protected:

public:
    UQLWeaponManager();

    void SetUser(AQLCharacter* Character);

    void AddWeapon(AQLWeapon* Weapon);

    void SetCurrentWeapon(const FName& WeaponName);

    AQLWeapon* GetCurrentWeapon();
protected:
    AQLCharacter* User;
    AQLWeapon* CurrentWeapon;
    TMap<FName, AQLWeapon*> WeaponList;
};
