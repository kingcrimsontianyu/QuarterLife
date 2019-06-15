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
#include "GameFramework/HUD.h"
#include "QLHUD.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLHUD : public AHUD
{
	GENERATED_BODY()

public:
    AQLHUD();

    // Primary draw call for the HUD
    virtual void DrawHUD() override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void UpdateCrossHair(UTexture2D* CrosshairTextureExt);

protected:
    UPROPERTY()
    TWeakObjectPtr<UTexture2D> CrosshairTexture;
};
