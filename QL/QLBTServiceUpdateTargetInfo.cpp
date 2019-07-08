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

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
}