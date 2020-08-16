//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLAIPerceptionComponent.h"
#include "QLUtility.h"
#include "Perception/AISense.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Prediction.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Team.h"
#include "QLAIController.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLAIPerceptionComponent::UQLAIPerceptionComponent(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{

}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLAIPerceptionComponent::HandleExpiredStimulus(FAIStimulus& StimulusStore)
{
    Super::HandleExpiredStimulus(StimulusStore);

    // handle the expired sight stimulus
    if (StimulusStore.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass()))
    {
        AQLAIController* QLAIController = Cast<AQLAIController>(AIOwner);
        if (QLAIController)
        {
            QLAIController->ResetTarget();
        }
    }
}