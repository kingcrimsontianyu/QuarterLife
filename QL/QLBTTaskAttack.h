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
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "QLBTTaskAttack.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API UQLBTTaskAttack : public UBTTask_BlackboardBase
{
    GENERATED_BODY()

public:
    //------------------------------------------------------------
    //------------------------------------------------------------
    UQLBTTaskAttack(const FObjectInitializer& ObjectInitializer);

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
