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
void UQLBTServiceUpdateTargetInfo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PlayerController)
    {
        return;
    }

    APawn* MyPawn = PlayerController->GetPawn();
    if (!MyPawn)
    {
        return;
    }

    AQLCharacter* MyCharacter = Cast<AQLCharacter>(MyPawn);
    if (!MyCharacter)
    {
        return;
    }

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

    // the blackboard must have variables TargetLocation (FVector) and IsTargetVisible (bool)
    if (BlackboardComponent)
    {
        const FName TargetLocationKeyName(TEXT("TargetLocation"));
        FVector TargetLocation = MyCharacter->GetActorLocation();
        BlackboardComponent->SetValueAsVector(TargetLocationKeyName, TargetLocation);

        const FName TargetVisibilityKeyName(TEXT("IsTargetVisible"));
        auto* ThirdPersonMesh = MyCharacter->GetThirdPersonMesh();
        if (!ThirdPersonMesh)
        {
            return;
        }

        bool bIsTargetVisible = ThirdPersonMesh->IsVisible();
        BlackboardComponent->SetValueAsBool(TargetVisibilityKeyName, bIsTargetVisible);
    }
}