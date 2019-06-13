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
#include "Components/TextBlock.h"
#include "QLUmgUserWidget.generated.h"

class AQLPlayerController;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS(Abstract)
class QL_API UQLUmgUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UQLUmgUserWidget(const FObjectInitializer& ObjectInitializer);

    // Optionally override the Blueprint "Event Construct" event
    virtual void NativeConstruct() override;

    // Optionally override the tick event
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void ShowDamageOnScreen(const FString& msg, const FVector& Location);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetQLPlayerController(AQLPlayerController* QLPlayerControllerExt);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    AQLPlayerController* GetQLPlayerController();

protected:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* DamageText0;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* DamageText1;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* DamageText2;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* DamageText3;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* DamageText4;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* DamageText5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
    UWidgetAnimation* FadeAnimationCPP0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
    UWidgetAnimation* FadeAnimationCPP1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
    UWidgetAnimation* FadeAnimationCPP2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
    UWidgetAnimation* FadeAnimationCPP3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
    UWidgetAnimation* FadeAnimationCPP4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
    UWidgetAnimation* FadeAnimationCPP5;

    UPROPERTY()
    TArray<UTextBlock*> DamageTextList;

    UPROPERTY()
    TArray<UWidgetAnimation*> FadeAnimationCPPList;

    UPROPERTY()
    UTextBlock* CurrentDamageText;

    UPROPERTY()
    UWidgetAnimation* CurrentFadeAnimationCPP;

    UPROPERTY()
    AQLPlayerController* QLPlayerController;

    int32 Counter;
};
