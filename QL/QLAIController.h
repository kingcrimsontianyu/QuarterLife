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
#include <vector>
#include "QLAIController.generated.h"

class UAISenseConfig;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Prediction;
class UAISenseConfig_Damage;
class UAISenseConfig_Team;
class AQLCharacter;

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

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    AQLCharacter* GetTarget();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void ResetTarget();

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void BroadcastTarget(AQLCharacter* Target);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FName GetStartingWeaponName();

protected:
    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

    //------------------------------------------------------------
    //DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPerceptionUpdatedDelegate, const TArray<AActor*>&, UpdatedActors);
    //DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActorPerceptionUpdatedDelegate, AActor*, Actor, FAIStimulus, Stimulus);
    //------------------------------------------------------------
    UFUNCTION()
    void OnPerceptionUpdatedImpl(const TArray<AActor*>& UpdatedActors);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    UBehaviorTree* BehaviorTreeBasic;

    UPROPERTY()
    UAISenseConfig_Sight* AISenseConfig_Sight;

    UPROPERTY()
    UAISenseConfig_Hearing* AISenseConfig_Hearing;

    UPROPERTY()
    UAISenseConfig_Prediction* AISenseConfig_Prediction;

    UPROPERTY()
    UAISenseConfig_Damage* AISenseConfig_Damage;

    UPROPERTY()
    UAISenseConfig_Team* AISenseConfig_Team;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    FGenericTeamId QLTeamId;

    UPROPERTY()
    TWeakObjectPtr<AQLCharacter> QLTarget;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    bool bRandomStartingWeapon;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    FName StartingWeaponName;

    std::vector<FName> StartingWeaponList;
};
