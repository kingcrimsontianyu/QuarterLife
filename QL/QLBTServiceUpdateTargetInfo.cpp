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

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PlayerController)
    {
        return;
    }

    APawn* TargetPawn = PlayerController->GetPawn();
    if (TargetPawn)
    {

        AQLCharacter* TargetCharacter = Cast<AQLCharacter>(TargetPawn);
        if (TargetCharacter)
        {
            // the blackboard must have namesake variables
            if (BlackboardComponent)
            {
                {
                    const FName TargetLocationKeyName(TEXT("TargetLocation"));
                    FVector TargetLocation = TargetCharacter->GetActorLocation();
                    BlackboardComponent->SetValueAsVector(TargetLocationKeyName, TargetLocation);
                }

                {
                    const FName TargetVisibilityKeyName(TEXT("IsTargetVisible"));
                    auto* ThirdPersonMesh = TargetCharacter->GetThirdPersonMesh();
                    if (!ThirdPersonMesh)
                    {
                        return;
                    }

                    bool bIsTargetVisible = ThirdPersonMesh->IsVisible();
                    BlackboardComponent->SetValueAsBool(TargetVisibilityKeyName, bIsTargetVisible);
                }

                {
                    const FName TargetAliveKeyName(TEXT("IsTargetAlive"));
                    bool bResult = TargetCharacter->IsAlive();
                    BlackboardComponent->SetValueAsBool(TargetAliveKeyName, bResult);
                }
            }
        }
    }
    // the target is dead and destroyed
    else
    {
        const FName TargetAliveKeyName(TEXT("IsTargetAlive"));
        BlackboardComponent->SetValueAsBool(TargetAliveKeyName, false);
    }
}