//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLBTTaskPatrol.h"
#include "QLAIController.h"
#include "QLCharacter.h"
#include "QLUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLBTTaskPatrol::UQLBTTaskPatrol(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
    NodeName = "Patrol";
}

//------------------------------------------------------------
//------------------------------------------------------------
EBTNodeResult::Type UQLBTTaskPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    auto* MyController = Cast<AQLAIController>(OwnerComp.GetAIOwner());
    if (!MyController)
    {
        return EBTNodeResult::Failed;
    }

    APawn* MyBot = MyController->GetPawn();
    if (MyBot)
    {
        AQLCharacter* MyBotCharacter = Cast<AQLCharacter>(MyBot);
        if (MyBotCharacter)
        {
            MyBotCharacter->SetMaxWalkSpeed(125.0f);
        }
    }

    return EBTNodeResult::Succeeded;
}