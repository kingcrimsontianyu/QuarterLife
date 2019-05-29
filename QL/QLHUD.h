//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QLProperty")
    UTexture2D* CrosshairTextureDefault;
protected:
    void DrawDefaultCrosshair();

    TArray<UTexture2D*> CrosshairTextureList;
};
