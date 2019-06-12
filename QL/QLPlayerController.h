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
#include "QLUmgUserWidget.h"
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
    float GetControlledPawnVelocityLength() const;

    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void AddUMG();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    UQLUmgUserWidget* GetUMG();

    UPROPERTY(EditDefaultsOnly, Category = "C++Property")
    TSubclassOf<UQLUmgUserWidget> UmgUserWidgetClass;



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

    UQLUmgUserWidget* UmgUserWidget;

    float FPS;
};
