//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLBTServiceUpdateTargetInfo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Kismet/GameplayStatics.h"
#include "QLCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "QLAIController.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLBTServiceUpdateTargetInfo::UQLBTServiceUpdateTargetInfo(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
    NodeName = "UpdateTargetInfo";
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLBTServiceUpdateTargetInfo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    auto* MyController = Cast<AQLAIController>(OwnerComp.GetAIOwner());

    if (!MyController)
    {
        return;
    }

    AQLCharacter* Target = MyController->GetTarget();

    // according to c++ standard, && performs short-circuit evaluation,
    // i.e. do not evaluate the second operand if the result is known after evaluating the first
    // thus it is safe to deference Target in the second operand
    bool bResult = Target && Target->QLIsVisible() && Target->IsAlive();
    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (BlackboardComponent)
    {
        BlackboardComponent->SetValueAsBool(FName(TEXT("CanAttackTarget")), bResult);
    }
}