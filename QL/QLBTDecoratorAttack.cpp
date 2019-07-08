//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLBTDecoratorAttack.h"
#include "QLAIController.h"
#include "QLCharacter.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLBTDecoratorAttack::UQLBTDecoratorAttack(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
    NodeName = "CanAttack";
}

//------------------------------------------------------------
//------------------------------------------------------------
bool UQLBTDecoratorAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    auto* MyController = Cast<AQLAIController>(OwnerComp.GetAIOwner());

    if (!MyController)
    {
        return false;
    }

    AQLCharacter* Target = MyController->GetTarget();

    if (Target && Target->QLIsVisible() && Target->IsAlive())
    {
        return true;
    }
    else
    {
        return false;
    }
}