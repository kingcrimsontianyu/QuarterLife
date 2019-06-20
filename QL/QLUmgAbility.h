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
#include "Blueprint/UserWidget.h"
#include "QLUmgAbility.generated.h"

class AQLPlayerController;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API UQLUmgAbility : public UUserWidget
{
	GENERATED_BODY()

public:
    UQLUmgAbility(const FObjectInitializer& ObjectInitializer);

    // Optionally override the Blueprint "Event Construct" event
    virtual void NativeConstruct() override;

    // Optionally override the tick event
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetQLPlayerController(AQLPlayerController* QLPlayerControllerExt);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    AQLPlayerController* GetQLPlayerController();
protected:
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);

    UPROPERTY()
    TWeakObjectPtr<AQLPlayerController> QLPlayerController;
};
