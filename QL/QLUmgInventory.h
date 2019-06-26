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
#include "QLUmgInventory.generated.h"

class AQLPlayerController;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API UQLUmgInventory : public UUserWidget
{
	GENERATED_BODY()

public:
    UQLUmgInventory(const FObjectInitializer& ObjectInitializer);

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

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++Function")
    int SetCurrentAbility(const FName& QLName);
protected:
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);

    UPROPERTY()
    TWeakObjectPtr<AQLPlayerController> QLPlayerController;
};
