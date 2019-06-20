//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLUmgAbility.h"
#include "QLPlayerController.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "QLUtility.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLUmgAbility::UQLUmgAbility(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{

}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLUmgAbility::NativeConstruct()
{
    // Do some custom setup

    // Call the Blueprint "Event Construct" node
    Super::NativeConstruct();
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLUmgAbility::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    // Make sure to call the base class's NativeTick function
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Do your custom tick stuff here
}


//------------------------------------------------------------
//------------------------------------------------------------
void UQLUmgAbility::SetQLPlayerController(AQLPlayerController* QLPlayerControllerExt)
{
    QLPlayerController = QLPlayerControllerExt;
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLPlayerController* UQLUmgAbility::GetQLPlayerController()
{
    return QLPlayerController.Get();
}

//------------------------------------------------------------
//------------------------------------------------------------
FReply UQLUmgAbility::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    Super::NativeOnKeyDown(InGeometry, InKeyEvent);

    if (QLPlayerController.IsValid())
    {
        auto* InputSettings = UInputSettings::GetInputSettings();
        if (InputSettings)
        {
            // hide ability menu
            TArray<FInputActionKeyMapping> Result;
            InputSettings->GetActionMappingByName("AbilityMenu", Result);

            bool bFound = false;
            for (auto&& Item : Result)
            {
                if (InKeyEvent.GetKey() == Item.Key)
                {
                    bFound = true;
                    break;
                }
            }

            if (bFound && IsInViewport())
            {
                // UmgAbility->RemoveFromViewport();
                // equivalently
                RemoveFromParent();

                QLPlayerController->bShowMouseCursor = false;
                QLPlayerController->SetInputMode(FInputModeGameOnly());

                // bring time back to normal
                float TimeDilation = 1.0f;
                UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
            }
        }
    }

    return FReply::Handled();
}