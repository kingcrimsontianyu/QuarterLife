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
#include "QLWeapon.h"

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

        AQLWeapon* CurrentWeapon = MyBotCharacter->GetCurrentWeapon();

        // if the current weapon is not set
        if (!CurrentWeapon)
        {
            MyBotCharacter->SetCurrentWeapon(MyController->GetStartingWeaponType());
        }
        // otherwise, ready to shoot
        else
        {
            FVector WhereToAim;

            // for projectile weapon, predict enemy movement
            if (CurrentWeapon->IsProjectileWeapon())
            {
                float TimeProjectileHitsEnemy;
                QLUtility::MakePredictionShot(
                    WhereToAim,
                    TimeProjectileHitsEnemy,
                    MyBotCharacter->GetActorLocation(),
                    Target->GetActorLocation(),
                    Target->GetVelocity(),
                    CurrentWeapon->GetProjectileSpeed());
            }
            // for non-projectile weapon, aim at the enemy
            else
            {
                WhereToAim = Target->GetTargetLocation();
            }

            // once the bot leaves attack task, MyController->ClearFocus(EAIFocusPriority::Gameplay) should be called
            MyController->SetFocalPoint(WhereToAim);
            MyBotCharacter->GetFirstPersonCameraComponent()->SetWorldRotation(MyController->GetControlRotation());
            MyBotCharacter->OnFire();
        }
    }

    return EBTNodeResult::Succeeded;
}
