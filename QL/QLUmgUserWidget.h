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
#include "QLUmgUserWidget.generated.h"

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

protected:

};
