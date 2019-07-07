//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLAIController.h"
#include "Classes/BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "QLCharacter.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLAIController::AQLAIController()
{

}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAIController::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (BehaviorTreeBasic)
    {
        RunBehaviorTree(BehaviorTreeBasic);
    }

    auto* MyCharacter = Cast<AQLCharacter>(InPawn);
    if (MyCharacter)
    {
        // allow bots to aim up and down (change pitch)
        MyCharacter->InitializeBot();
    }
}
