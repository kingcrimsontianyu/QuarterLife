//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLPlayerController.h"
#include "QLUtility.h"
#include "QLCharacter.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLPlayerController::AQLPlayerController() :
FPS(0.0f)
{
    // ui
    UmgUserWidgetClass = UQLUmgUserWidget::StaticClass();
    UmgUserWidget = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLPlayerController::GetFrameRate() const
{
    return FPS;
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLPlayerController::GetControlledPawnVelocityLength() const
{
    APawn* ControlledPawn = GetPawn();
    if (ControlledPawn)
    {
        // !!! note that the pawn must have movement component in order to calculate velocity
        // !!! otherwise the velocity is always zero.
        // !!! todo: for super power mimic matter, the pawn does not use movement component by design
        // !!! how to obtain the velocity then?
        FVector temp = ControlledPawn->GetVelocity();
        return temp.Size();
    }
    else
    {
        // UE_LOG();
        return 0.0;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    FPS = 1.0 / DeltaSeconds;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::AddUMG()
{
    UmgUserWidget = CreateWidget<UQLUmgUserWidget>(GetWorld(), UmgUserWidgetClass, FName("UmgUserWidget"));
    UmgUserWidget->AddToViewport();
    bShowMouseCursor = false;
    SetInputMode(FInputModeGameOnly());
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::BeginPlay()
{
    Super::BeginPlay();

    AddUMG();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::OnPossess(APawn* ControlledPawn)
{
    Super::OnPossess(ControlledPawn);

    AQLCharacter* ControlledCharacter = Cast<AQLCharacter>(ControlledPawn);
    if (ControlledCharacter)
    {
        // controlled character does not see his own health and armor bar
        ControlledCharacter->SetHealthArmorBarVisible(false);
    }
}
