//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLPlayerHealthArmorBarUserWidget.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLPlayerHealthArmorBarUserWidget::UQLPlayerHealthArmorBarUserWidget(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{

}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLPlayerHealthArmorBarUserWidget::NativeConstruct()
{
    // Do some custom setup

    // Call the Blueprint "Event Construct" node
    Super::NativeConstruct();
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLPlayerHealthArmorBarUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    // Make sure to call the base class's NativeTick function
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Do your custom tick stuff here
}

//------------------------------------------------------------
//------------------------------------------------------------
float UQLPlayerHealthArmorBarUserWidget::UpdateHealthBar_Implementation(float HealthPercent)
{
    return HealthPercent;
}

//------------------------------------------------------------
//------------------------------------------------------------
float UQLPlayerHealthArmorBarUserWidget::UpdateArmorBar_Implementation(float ArmorPercent)
{
    return ArmorPercent;
}