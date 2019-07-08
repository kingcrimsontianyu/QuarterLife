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
void UQLBTServiceUpdateTargetInfo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    auto* MyController = Cast<AQLAIController>(OwnerComp.GetAIOwner());

    if (!MyController)
    {
        return;
    }

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (BlackboardComponent)
    {
        AQLCharacter* Target = MyController->GetTarget();
        bool bResult = Target && Target->QLGetVisibility() && Target->IsAlive();
        BlackboardComponent->SetValueAsBool(FName(TEXT("CanAttackTarget")), bResult);

        if (Target)
        {
            BlackboardComponent->SetValueAsObject(FName(TEXT("Target")), Target);
        }
    }
}