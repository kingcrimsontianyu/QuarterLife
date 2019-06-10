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
#include "QLWeapon.h"
#include "QLWeaponRailGun.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLWeaponRailGun : public AQLWeapon
{
	GENERATED_BODY()

public:
    AQLWeaponRailGun();

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    int Ammo;
};
