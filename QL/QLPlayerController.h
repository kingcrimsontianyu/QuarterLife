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
#include "QLUmgFirstPerson.h"
#include "QLPlayerController.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLPlayerController : public APlayerController
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

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void ShowDamageOnScreen(float DamageAmount, const FVector& WorldTextLocation);

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

    UPROPERTY()
    UQLUmgFirstPerson* UmgFirstPerson;

    float FPS;
};
