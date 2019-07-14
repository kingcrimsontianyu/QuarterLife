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
#include "Classes/Perception/AISense.h"
#include "Classes/Perception/AISense_Sight.h"
#include "Classes/Perception/AISense_Hearing.h"
#include "Classes/Perception/AISense_Prediction.h"
#include "Classes/Perception/AISense_Damage.h"
#include "Classes/Perception/AISense_Team.h"
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