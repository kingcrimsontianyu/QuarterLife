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
#include "QLBTTaskFollowTarget.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API UQLBTTaskFollowTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
