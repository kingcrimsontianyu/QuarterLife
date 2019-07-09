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
#include "QLUmgFirstPerson.h"
#include "QLUmgInventory.h"
#include "Kismet/GameplayStatics.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLPlayerController::AQLPlayerController() :
FPS(0.0f)
{
    // ui
    UmgFirstPersonClass = UQLUmgFirstPerson::StaticClass();
    UmgFirstPerson = nullptr;

    UmgInventoryClass = UQLUmgInventory::StaticClass();
    UmgInventory = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLPlayerController::GetFrameRate() const
{
    return FPS;
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLPlayerController::GetControlledPawnSpeed() const
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
        return 0.0f;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    FPS = 1.0f / DeltaSeconds;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::AddUMG()
{
    UmgFirstPerson = CreateWidget<UQLUmgFirstPerson>(GetWorld(), UmgFirstPersonClass, FName(TEXT("UmgFirstPerson")));
    UmgFirstPerson->QLSetPlayerController(this);
    UmgFirstPerson->AddToViewport();
    bShowMouseCursor = false;
    SetInputMode(FInputModeGameOnly());

    UmgInventory = CreateWidget<UQLUmgInventory>(GetWorld(), UmgInventoryClass, FName(TEXT("UmgInventory")));
    UmgInventory->QLSetPlayerController(this);
}

//------------------------------------------------------------
//------------------------------------------------------------
UQLUmgFirstPerson* AQLPlayerController::GetUMG()
{
    return UmgFirstPerson;
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

        AddUMG();

        if (UmgFirstPerson)
        {
            UmgFirstPerson->UpdateTextHealthValue(ControlledCharacter->GetHealth());
            UmgFirstPerson->UpdateTextArmorValue(ControlledCharacter->GetArmor());
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::ShowDamageOnScreen(float DamageAmount, const FVector& WorldTextLocation)
{
    if (!UmgFirstPerson)
    {
        return;
    }

    int32 DamageAmountInt = FMath::RoundToInt(DamageAmount);
    UmgFirstPerson->ShowDamageOnScreen(FString::FromInt(DamageAmountInt), WorldTextLocation);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::ShowAbilityMenu()
{
    if (UmgInventory && !UmgInventory->IsInViewport())
    {
        UmgInventory->AddToViewport();

        bShowMouseCursor = true;
        SetInputMode(FInputModeUIOnly());
        UmgInventory->bIsFocusable = true;
        UmgInventory->SetKeyboardFocus();

        // slow down time
        float TimeDilation = 0.1f;
        UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("AbilityMenu", EInputEvent::IE_Pressed, this, &AQLPlayerController::ShowAbilityMenu);
    InputComponent->BindAction("RestartLevel", EInputEvent::IE_Pressed, this, &AQLPlayerController::OnRestartLevel);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::OnRestartLevel()
{
    UGameplayStatics::OpenLevel(GetWorld(), "QLArena");
}