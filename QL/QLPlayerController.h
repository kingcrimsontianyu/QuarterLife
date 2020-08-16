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
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "QLPlayerController.generated.h"

class UQLUmgFirstPerson;
class UQLUmgInventory;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
    AQLPlayerController();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetFrameRate() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetControlledPawnSpeed() const;

    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void AddUMG();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    UQLUmgFirstPerson* GetUMG();

    UPROPERTY(EditDefaultsOnly, Category = "C++Property")
    TSubclassOf<UQLUmgFirstPerson> UmgFirstPersonClass;

    UPROPERTY(EditDefaultsOnly, Category = "C++Property")
    TSubclassOf<UQLUmgInventory> UmgInventoryClass;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void ShowDamageOnScreen(float DamageAmount, const FVector& WorldTextLocation);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void ShowAbilityMenu();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void OnRestartLevel();

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual FGenericTeamId GetGenericTeamId() const override;
protected:
    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void BeginPlay() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void OnPossess(APawn* ControlledPawn) override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void SetupInputComponent() override;

    UPROPERTY()
    UQLUmgFirstPerson* UmgFirstPerson;

    UPROPERTY()
    UQLUmgInventory* UmgInventory;

    float FPS;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    FGenericTeamId QLTeamId;
};
