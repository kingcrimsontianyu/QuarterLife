//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLUmgUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanelSlot.h"
#include "QLPlayerController.h"
#include "QLUtility.h"
#include "Engine/World.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLUmgUserWidget::UQLUmgUserWidget(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer),
DamageText0(nullptr),
DamageText1(nullptr),
DamageText2(nullptr),
DamageText3(nullptr),
DamageText4(nullptr),
DamageText5(nullptr),
FadeAnimationCPP0(nullptr),
FadeAnimationCPP1(nullptr),
FadeAnimationCPP2(nullptr),
FadeAnimationCPP3(nullptr),
FadeAnimationCPP4(nullptr),
FadeAnimationCPP5(nullptr),
Counter(0)
{

}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLUmgUserWidget::NativeConstruct()
{
    // Do some custom setup

    // Call the Blueprint "Event Construct" node
    Super::NativeConstruct();

    if (DamageText0 && DamageText1 && DamageText2 && DamageText3 && DamageText4 && DamageText5)
    {
        DamageTextList.Add(DamageText0);
        DamageTextList.Add(DamageText1);
        DamageTextList.Add(DamageText2);
        DamageTextList.Add(DamageText3);
        DamageTextList.Add(DamageText4);
        DamageTextList.Add(DamageText5);

        CurrentDamageText = DamageTextList[0];
    }

    if (FadeAnimationCPP0 && FadeAnimationCPP1 && FadeAnimationCPP2 && FadeAnimationCPP3 && FadeAnimationCPP4 && FadeAnimationCPP5)
    {
        FadeAnimationCPPList.Add(FadeAnimationCPP0);
        FadeAnimationCPPList.Add(FadeAnimationCPP1);
        FadeAnimationCPPList.Add(FadeAnimationCPP2);
        FadeAnimationCPPList.Add(FadeAnimationCPP3);
        FadeAnimationCPPList.Add(FadeAnimationCPP4);
        FadeAnimationCPPList.Add(FadeAnimationCPP5);

        CurrentFadeAnimationCPP = FadeAnimationCPPList[0];
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLUmgUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    // Make sure to call the base class's NativeTick function
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Do your custom tick stuff here
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLUmgUserWidget::ShowDamageOnScreen(const FString& msg, const FVector& Location)
{
    // update pointer
    CurrentDamageText = DamageTextList[Counter];
    CurrentFadeAnimationCPP = FadeAnimationCPPList[Counter];

    // set perturbation
    float temp = Counter * 10.0f;
    float PerturbationX = Counter % 2 == 0 ? temp : -temp;
    float PerturbationY = -temp;

    // convert coordinates
    FVector2D ScreenLocation;
    // todo: it appears that ProjectWorldLocationToScreen() only works well in full screen mode
    // not in PIE. need to investigate.
    bool bIsConversionSuccessful = QLPlayerController->ProjectWorldLocationToScreen(Location, ScreenLocation);
    if (!bIsConversionSuccessful)
    {
        return;
    }

    if (!CurrentDamageText.IsValid() || !CurrentFadeAnimationCPP.IsValid())
    {
        return;
    }

    CurrentDamageText->SetText(FText::FromString(msg));

    UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(CurrentDamageText->Slot);
    ScreenLocation.X += PerturbationX;
    ScreenLocation.Y += PerturbationY;
    CanvasSlot->SetPosition(ScreenLocation);

    // display animation
    if (!CurrentFadeAnimationCPP.IsValid())
    {
        return;
    }

    PlayAnimation(CurrentFadeAnimationCPP.Get());

    // update counter
    ++Counter;
    if (Counter >= DamageTextList.Num())
    {
        Counter = 0;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLUmgUserWidget::SetQLPlayerController(AQLPlayerController* QLPlayerControllerExt)
{
    QLPlayerController = QLPlayerControllerExt;
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLPlayerController* UQLUmgUserWidget::GetQLPlayerController()
{
    return QLPlayerController.Get();
}

//------------------------------------------------------------
//------------------------------------------------------------
float UQLUmgUserWidget::UpdateTextHealthValue_Implementation(float Health)
{
    return Health;
}

//------------------------------------------------------------
//------------------------------------------------------------
float UQLUmgUserWidget::UpdateTextArmorValue_Implementation(float Armor)
{
    return Armor;
}

//------------------------------------------------------------
//------------------------------------------------------------
float UQLUmgUserWidget::UpdateTextFPSValue_Implementation()
{
    float FPS = 0.0f;

    if (QLPlayerController.IsValid())
    {
        FPS = QLPlayerController->GetFrameRate();
    }

    return FPS;
}

//------------------------------------------------------------
//------------------------------------------------------------
float UQLUmgUserWidget::UpdateTextSpeedValue_Implementation()
{
    float Speed = 0.0f;

    if (QLPlayerController.IsValid())
    {
        Speed = QLPlayerController->GetControlledPawnSpeed();
    }

    return Speed;
}