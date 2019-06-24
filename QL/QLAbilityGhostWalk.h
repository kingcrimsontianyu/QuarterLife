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
#include "QLAbility.h"
#include "Components/TimelineComponent.h"
#include "QLAbilityGhostWalk.generated.h"

class UPostProcessComponent;
//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLAbilityGhostWalk : public AQLAbility
{
	GENERATED_BODY()

public:
    AQLAbilityGhostWalk();

    virtual void OnUse() override;

    virtual void OnAbilityEnd() override;
protected:
    //------------------------------------------------------------
    // Called when the game starts or when spawned
    //------------------------------------------------------------
    virtual void BeginPlay() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++Property")
    UPostProcessComponent* PostProcessComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float TelefragDamage;
};
