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
#include "GameFramework/CharacterMovementComponent.h"
#include "QLMoveComponentQuake.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API UQLMoveComponentQuake : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    UQLMoveComponentQuake();

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void PostInitProperties() override;
};
