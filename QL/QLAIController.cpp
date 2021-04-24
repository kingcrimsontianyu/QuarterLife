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
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "QLCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Prediction.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Team.h"
#include "Perception/AISense.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Prediction.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Team.h"
#include "QLUtility.h"
#include "QLAIPerceptionComponent.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLAIController::AQLAIController()
{
    // AI sense
    AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
    AISenseConfig_Sight->SightRadius = 3500.0f;
    AISenseConfig_Sight->LoseSightRadius = 4000.0f;
    AISenseConfig_Sight->PeripheralVisionAngleDegrees = 90.0f;
    AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
    AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
    AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
    AISenseConfig_Sight->SetMaxAge(6.0f);

    AISenseConfig_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AISenseConfig_Hearing"));

    AISenseConfig_Prediction = CreateDefaultSubobject<UAISenseConfig_Prediction>(TEXT("AISenseConfig_Prediction"));

    AISenseConfig_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("AISenseConfig_Damage"));
    AISenseConfig_Damage->SetMaxAge(6.0f); // after the set duration, damage stimulus expires

    AISenseConfig_Team = CreateDefaultSubobject<UAISenseConfig_Team>(TEXT("AISenseConfig_Team"));

    PerceptionComponent = CreateDefaultSubobject<UQLAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
    PerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
    PerceptionComponent->ConfigureSense(*AISenseConfig_Hearing);
    PerceptionComponent->ConfigureSense(*AISenseConfig_Prediction);
    PerceptionComponent->ConfigureSense(*AISenseConfig_Damage);
    PerceptionComponent->ConfigureSense(*AISenseConfig_Team);

    PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());

    QLTeamId = FGenericTeamId(1);

    StartingWeaponList.Add(EQLWeapon::RailGun);
    StartingWeaponList.Add(EQLWeapon::LightningGun);
    StartingWeaponList.Add(EQLWeapon::RocketLauncher);
    StartingWeaponList.Add(EQLWeapon::NailGun);

    bRandomStartingWeapon = false;
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
// in the ctor, detection of friendlies and neutrals are disabled.
// so in this function UpdatedActors are always enemies.
//------------------------------------------------------------
void AQLAIController::OnPerceptionUpdatedImpl(const TArray<AActor*>& UpdatedActors)
{
    APawn* Bot = GetPawn();
    if (!Bot)
    {
        return;
    }

    // for each target actor that has been sensed
    for (auto&& Target : UpdatedActors)
    {
        auto* TargetCharacter = Cast<AQLCharacter>(Target);
        if (!TargetCharacter)
        {
            continue;
        }

        bool bEnemySensed = false;

        FActorPerceptionBlueprintInfo Info;
        // retrieve what has been sensed about the target actor
        PerceptionComponent->GetActorsPerception(Target, Info);

        // for each stimulus
        for (auto&& Stimulus : Info.LastSensedStimuli)
        {
            // check if the bot has seen the player
            // OnPerceptionUpdated() is fired when the player enters or leaves the region of sight, or the sense expires
            if (Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass()))
            {
                // currently in UE 4.22.3, OnPerceptionUpdated() never reports sight expiration event
                // as a workaround, bEnemySensed is set to true whether the player target enters or leaves the region of sight
                // the target is reset when the sight sense expires, which is handled in PerceptionComponent->HandleExpiredStimulus()
                //if (Stimulus.WasSuccessfullySensed() && GetTeamAttitudeTowards(*TargetCharacter) == ETeamAttitude::Hostile)
                //{
                //    bEnemySensed = true;
                //}

                bEnemySensed = true;
            }

            // check if the bot has taken damage
            else if (Stimulus.Type == UAISense::GetSenseID(UAISense_Damage::StaticClass()))
            {
                if (Stimulus.WasSuccessfullySensed() && !Stimulus.IsExpired() && GetTeamAttitudeTowards(*TargetCharacter) == ETeamAttitude::Hostile)
                {
                    bEnemySensed = true;
                }
            }

            // check if any teammate is nearby
            else if (Stimulus.Type == UAISense::GetSenseID(UAISense_Team::StaticClass()))
            {
                QLUtility::Screen("TEAMMATE");
            }
        }

        if (bEnemySensed)
        {
            QLTarget = TargetCharacter;
            BroadcastTarget(QLTarget.Get());
        }
        else
        {
            QLTarget.Reset();
        }
    }

    // update blackboard variable
    if (Blackboard)
    {
        // according to c++ standard, operator && performs short-circuit evaluation,
        // i.e. do not evaluate the second operand if the result is known after evaluating the first
        // thus it is safe to deference Target in the second operand
        bool bResult = QLTarget.IsValid() && QLTarget->QLGetVisibility() && QLTarget->IsAlive();
        Blackboard->SetValueAsBool(FName(TEXT("CanAttackTarget")), bResult);

        if (QLTarget.IsValid())
        {
            Blackboard->SetValueAsObject(FName(TEXT("Target")), QLTarget.Get());
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLCharacter* AQLAIController::GetTarget()
{
    return QLTarget.Get();
}

//------------------------------------------------------------
//------------------------------------------------------------
ETeamAttitude::Type AQLAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
    const AQLCharacter* QLCharacter = Cast<AQLCharacter>(&Other);

    if (!QLCharacter)
    {
        return ETeamAttitude::Neutral;
    }

    const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(QLCharacter->GetController());
    if (!TeamAgent)
    {
        return ETeamAttitude::Neutral;
    }

    // in QL, 0 is reserved for the player team id
    if (TeamAgent->GetGenericTeamId() == 0)
    {
        return ETeamAttitude::Hostile;
    }
    else
    {
        return ETeamAttitude::Friendly;
    }

    // return Super::GetTeamAttitudeTowards(*QLCharacter->GetController());
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAIController::BroadcastTarget(AQLCharacter* Target)
{
    if (Target)
    {
        float EventRange = 1000.0f;
        FAITeamStimulusEvent TeamStimulusEvent(this, // broadcaster
            Target, // enemy
            Target->GetActorLocation(), // last known location
            EventRange // event range
        );
        UAIPerceptionSystem::OnEvent<FAITeamStimulusEvent, FAITeamStimulusEvent::FSenseClass>(GetWorld(), TeamStimulusEvent);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
EQLWeapon AQLAIController::GetStartingWeaponType()
{
    if (bRandomStartingWeapon)
    {
        StartingWeaponType = StartingWeaponList[FMath::RandRange(0, StartingWeaponList.Num() - 1)];
    }

    return StartingWeaponType;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAIController::ResetTarget()
{
    QLTarget.Reset();
}