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

    AQLCharacter* GetUser();

    void AddWeapon(AQLWeapon* Weapon);

    void SetCurrentWeapon(const FName& QLName);

    void UpdateCrossHair();

    AQLWeapon* GetCurrentWeapon();

    void SetDamageMultiplier(const float Value);

    void StartGlowWeapon(const FLinearColor& Color);

    void StopGlowWeapon();

    void CreateAndAddAllWeapons(const TArray<TSubclassOf<AQLWeapon>>& WeaponClassList);

    void SetCurrentWeaponVisibility(const bool bFlag);

    bool HasWeapon(const FName& WeaponName);
protected:
    // do not use UPROPERTY() here
    // it breaks the character weapon system
    // to do: need to understand why
    TWeakObjectPtr<AQLCharacter> User;

    UPROPERTY()
    TWeakObjectPtr<AQLWeapon> CurrentWeapon;

    UPROPERTY()
    TArray<AQLWeapon*> WeaponList;

    UPROPERTY()
    float DamageMultiplier;

    UPROPERTY()
    bool bIsGlowing;

    UPROPERTY()
    FLinearColor GlowColor;
};
