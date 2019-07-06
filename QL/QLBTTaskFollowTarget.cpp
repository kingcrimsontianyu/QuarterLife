//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLBTTaskFollowTarget.h"
#include "QLAIController.h"
#include "QLCharacter.h"
#include "QLUtility.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "GameFramework/CharacterMovementComponent.h"

//------------------------------------------------------------
//------------------------------------------------------------
EBTNodeResult::Type UQLBTTaskFollowTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
            MyBotCharacter->ResetMaxWalkSpeed();
        }
    }

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

    // the blackboard must have variables TargetLocation (FVector) and IsTargetVisible (bool)
    if (BlackboardComponent)
    {
        const FName TargetLocationKeyName(TEXT("TargetLocation"));
        FVector TargetLocation = BlackboardComponent->GetValueAsVector(TargetLocationKeyName);

        float AcceptanceRadius = 500.0f;
        MyController->MoveToLocation(TargetLocation, AcceptanceRadius);
    }

    return EBTNodeResult::Succeeded;
}