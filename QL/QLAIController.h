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
#include "AIController.h"
#include "QLAIController.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLAIController : public AAIController
{
	GENERATED_BODY()

public:
    //------------------------------------------------------------
    //------------------------------------------------------------
    AQLAIController();

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void BeginPlay() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void OnPossess(APawn* InPawn) override;

    //------------------------------------------------------------
    // In the base class, this function simply zeros out pitch. WTF?
    // To allow bots to aim up and down, this function must therefore be overridden.
    //------------------------------------------------------------
    virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;

protected:
    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    UBehaviorTree* BehaviorTreeBasic;
};
