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
#include "QLAbilityTimeTravel.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLAbilityTimeTravel : public AQLAbility
{
	GENERATED_BODY()

public:
    AQLAbilityTimeTravel();

    virtual void OnUse() override;

    virtual void OnAbilityEnd() override;

    void SetNearAndFarActors(AActor* NearActorExt, AActor* FarActorExt);

protected:
    //------------------------------------------------------------
    // Called when the game starts or when spawned
    //------------------------------------------------------------
    virtual void BeginPlay() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

    void SwapNearAndFarActor();

protected:
    UPROPERTY()
    TWeakObjectPtr<AActor> NearActor;

    UPROPERTY()
    TWeakObjectPtr<AActor> FarActor;
};
