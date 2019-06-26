//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLUmgInventory.h"
#include "QLPlayerController.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "QLUtility.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLUmgInventory::UQLUmgInventory(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{

}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLUmgInventory::NativeConstruct()
{
    // Do some custom setup

    // Call the Blueprint "Event Construct" node
    Super::NativeConstruct();
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLUmgInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    // Make sure to call the base class's NativeTick function
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Do your custom tick stuff here
}


//------------------------------------------------------------
//------------------------------------------------------------
void UQLUmgInventory::SetQLPlayerController(AQLPlayerController* QLPlayerControllerExt)
{
    QLPlayerController = QLPlayerControllerExt;
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLPlayerController* UQLUmgInventory::GetQLPlayerController()
{
    return QLPlayerController.Get();
}

//------------------------------------------------------------
//------------------------------------------------------------
FReply UQLUmgInventory::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
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
                // UmgInventory->RemoveFromViewport();
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

//------------------------------------------------------------
//------------------------------------------------------------
int UQLUmgInventory::SetCurrentAbility_Implementation(const FName& QLName)
{
    if (QLPlayerController.IsValid())
    {
        auto* Pawn = QLPlayerController->GetPawn();
        auto* Character = Cast<AQLCharacter>(Pawn);
        if (Character)
        {
            Character->SetCurrentAbility(QLName);
        }
    }

    return 1;
}