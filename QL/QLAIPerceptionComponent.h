//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "QLAIPerceptionComponent.generated.h"

class AQLAIController;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API UQLAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
    UQLAIPerceptionComponent(const FObjectInitializer & ObjectInitializer);

    virtual void HandleExpiredStimulus(FAIStimulus& StimulusStore) override;
};
