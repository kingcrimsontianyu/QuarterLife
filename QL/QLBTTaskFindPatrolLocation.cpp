//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLBTTaskFindPatrolLocation.h"
#include "QLAIController.h"
#include "QLCharacter.h"
#include "QLUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "NavigationSystem.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLBTTaskFindPatrolLocation::UQLBTTaskFindPatrolLocation(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
    NodeName = "FindPatrolLocation";
}

//------------------------------------------------------------
//------------------------------------------------------------
EBTNodeResult::Type UQLBTTaskFindPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

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

    // get a random location
    FVector RandomLocation;
    float SearchRadius = 1000.0f;
    UNavigationSystemV1::K2_GetRandomReachablePointInRadius(MyController, MyBotCharacter->GetActorLocation(), RandomLocation, SearchRadius);

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (BlackboardComponent)
    {
        BlackboardComponent->SetValueAsVector(FName(TEXT("PatrolLocation")), RandomLocation);
    }

    MyBotCharacter->SetMaxWalkSpeed(125.0f);

    return EBTNodeResult::Succeeded;
}