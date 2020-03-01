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
#include "UObject/NoExportTypes.h"
#include "QLAbilityManager.generated.h"

class AQLCharacter;
class AQLAbility;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS(Blueprintable)
class QL_API UQLAbilityManager : public UObject
{
	GENERATED_BODY()

public:
    UQLAbilityManager();

    void SetUser(AQLCharacter* Character);

    AQLCharacter* GetUser();

    void AddAbility(AQLAbility* Ability);

    void SetCurrentAbility(const FName& QLName);

    AQLAbility* GetCurrentAbility();

    void SetDamageMultiplier(const float Value);

    void CreateAndAddAllAbilities(const TArray<TSubclassOf<AQLAbility>>& AbilityClassList);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetNearAndFarActors(AActor* NearActorExt, AActor* FarActorExt);
protected:
    TWeakObjectPtr<AQLCharacter> User;

    UPROPERTY()
    TWeakObjectPtr<AQLAbility> CurrentAbility;

    UPROPERTY()
    TArray<AQLAbility*> AbilityList;

    UPROPERTY()
    float DamageMultiplier;
};





