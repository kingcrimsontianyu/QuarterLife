//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLBTTaskAttack.h"
#include "QLAIController.h"
#include "QLCharacter.h"
#include "QLUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Kismet/GameplayStatics.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLBTTaskAttack::UQLBTTaskAttack(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
    NodeName = "Attack";
}

//------------------------------------------------------------
//------------------------------------------------------------
EBTNodeResult::Type UQLBTTaskAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (BlackboardComponent)
    {
        AQLCharacter* Target = MyController->GetTarget();
        if (!Target)
        {
            return EBTNodeResult::Failed;
        }

        //MyController->SetFocalPoint(Target->GetTargetLocation());
        MyBotCharacter->SetCurrentWeapon(FName(TEXT("RocketLauncher")));
        MyBotCharacter->OnFire();
    }

    return EBTNodeResult::Succeeded;
}