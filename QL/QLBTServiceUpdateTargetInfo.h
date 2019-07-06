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
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "QLBTServiceUpdateTargetInfo.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API UQLBTServiceUpdateTargetInfo : public UBTService_BlackboardBase
{
    GENERATED_BODY()

public:
    //------------------------------------------------------------
    //------------------------------------------------------------
    UQLBTServiceUpdateTargetInfo(const FObjectInitializer& ObjectInitializer);

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory, float DeltaSeconds) override;
};
