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
#include "QLPlayerHealthArmorBarUserWidget.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS(Abstract)
class QL_API UQLPlayerHealthArmorBarUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UQLPlayerHealthArmorBarUserWidget(const FObjectInitializer& ObjectInitializer);

    // Optionally override the Blueprint "Event Construct" event
    virtual void NativeConstruct() override;

    // Optionally override the tick event
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++Function")
    float UpdateHealthBar(float HealthPercent);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++Function")
    float UpdateArmorBar(float ArmorPercent);
protected:
};
