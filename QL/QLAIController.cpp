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
#include "Classes/Perception/AIPerceptionComponent.h"
#include "Classes/Perception/AISenseConfig.h"
#include "Classes/Perception/AISenseConfig_Sight.h"
#include "Classes/Perception/AISenseConfig_Hearing.h"
#include "Classes/Perception/AISenseConfig_Prediction.h"
#include "Classes/Perception/AISenseConfig_Damage.h"
#include "Classes/Perception/AISenseConfig_Team.h"
#include "Classes/Perception/AISense.h"
#include "Classes/Perception/AISense_Sight.h"
#include "Classes/Perception/AISense_Hearing.h"
#include "Classes/Perception/AISense_Prediction.h"
#include "Classes/Perception/AISense_Damage.h"
#include "Classes/Perception/AISense_Team.h"
#include "QLUtility.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLAIController::AQLAIController()
{
    // AI sense
    AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
    AISenseConfig_Sight->SightRadius = 6000.0f;
    AISenseConfig_Sight->LoseSightRadius = 7000.0f;
    AISenseConfig_Sight->PeripheralVisionAngleDegrees = 90.0f;

    AISenseConfig_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AISenseConfig_Hearing"));
    AISenseConfig_Prediction = CreateDefaultSubobject<UAISenseConfig_Prediction>(TEXT("AISenseConfig_Prediction"));
    AISenseConfig_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("AISenseConfig_Damage"));
    AISenseConfig_Team = CreateDefaultSubobject<UAISenseConfig_Team>(TEXT("AISenseConfig_Team"));

    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
    PerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
    PerceptionComponent->ConfigureSense(*AISenseConfig_Hearing);
    PerceptionComponent->ConfigureSense(*AISenseConfig_Prediction);
    PerceptionComponent->ConfigureSense(*AISenseConfig_Damage);
    PerceptionComponent->ConfigureSense(*AISenseConfig_Team);

    PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());

    QLTeamId = FGenericTeamId(1);

    QLDetectEnemies = true;
    QLDetectFriendlies = true;
    QLDetectNeutrals = true;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAIController::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAIController::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (PerceptionComponent)
    {
        PerceptionComponent->OnPerceptionUpdated.RemoveDynamic(this, &AQLAIController::OnPerceptionUpdatedImpl);
        PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AQLAIController::OnPerceptionUpdatedImpl);
    }

    if (AISenseConfig_Sight)
    {
        AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = QLDetectEnemies;
        AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = QLDetectFriendlies;
        AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = QLDetectNeutrals;
    }

    SetGenericTeamId(QLTeamId);
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
}


//------------------------------------------------------------
//------------------------------------------------------------
void AQLAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
    APawn* const MyPawn = GetPawn();
    if (MyPawn)
    {
        FRotator NewControlRotation = GetControlRotation();

        // Look toward focus
        const FVector FocalPoint = GetFocalPoint();
        if (FAISystem::IsValidLocation(FocalPoint))
        {
            NewControlRotation = (FocalPoint - MyPawn->GetPawnViewLocation()).Rotation();
        }
        else if (bSetControlRotationFromPawnOrientation)
        {
            NewControlRotation = MyPawn->GetActorRotation();
        }

        // This code snippet must be commented out in order for the bots to aim up and down
        // Don't pitch view unless looking at another pawn
        //if (NewControlRotation.Pitch != 0 && Cast<APawn>(GetFocusActor()) == nullptr)
        //{
        //    NewControlRotation.Pitch = 0.f;
        //}

        SetControlRotation(NewControlRotation);

        if (bUpdatePawn)
        {
            const FRotator CurrentPawnRotation = MyPawn->GetActorRotation();

            if (CurrentPawnRotation.Equals(NewControlRotation, 1e-3f) == false)
            {
                MyPawn->FaceRotation(NewControlRotation, DeltaTime);
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAIController::OnPerceptionUpdatedImpl(const TArray<AActor*>& UpdatedActors)
{
    for (auto&& Target : UpdatedActors)
    {
        auto* MyCharacter = Cast<AQLCharacter>(Target);
        if (!MyCharacter || MyCharacter->IsBot())
        {
            continue;
        }

        FActorPerceptionBlueprintInfo Info;
        PerceptionComponent->GetActorsPerception(Target, Info);

        for (const auto& Stimulus : Info.LastSensedStimuli)
        {
            // check if the bot has seen the player
            if (Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass()))
            {
                bool bSenseResult = Stimulus.WasSuccessfullySensed();
                if (bSenseResult)
                {
                    QLTarget = MyCharacter;
                    break;
                }
                else
                {
                    QLTarget.Reset();
                }
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLCharacter* AQLAIController::GetTarget()
{
    return QLTarget.Get();
}