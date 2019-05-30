//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------


#include "QLUmgUserWidget.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLUmgUserWidget::UQLUmgUserWidget(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer),
FPS(0.0f)
{

}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLUmgUserWidget::NativeConstruct()
{
    // Do some custom setup

    // Call the Blueprint "Event Construct" node
    Super::NativeConstruct();
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLUmgUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    // Make sure to call the base class's NativeTick function
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Do your custom tick stuff here
    FPS = 1.0 / InDeltaTime;
}

//------------------------------------------------------------
//------------------------------------------------------------
float UQLUmgUserWidget::GetFrameRate()
{
    return FPS;
}