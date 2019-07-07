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
UQLBTTaskFollowTarget::UQLBTTaskFollowTarget(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
    NodeName = "FollowTarget";
}

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
    if (!MyBot)
    {
        return EBTNodeResult::Failed;
    }

    AQLCharacter* MyBotCharacter = Cast<AQLCharacter>(MyBot);
    if (!MyBotCharacter)
    {
        return EBTNodeResult::Failed;
    }

    MyBotCharacter->ResetMaxWalkSpeed();

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

    // the blackboard must have variables TargetLocation (FVector) and IsTargetVisible (bool)
    if (BlackboardComponent)
    {
        const FName TargetLocationKeyName(TEXT("TargetLocation"));
        FVector TargetLocation = BlackboardComponent->GetValueAsVector(TargetLocationKeyName);

        float AcceptanceRadius = 800.0f;
        MyController->MoveToLocation(TargetLocation, AcceptanceRadius);
    }

    return EBTNodeResult::Succeeded;
}