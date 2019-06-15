//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLHUD::AQLHUD()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLHUD::DrawHUD()
{
    Super::DrawHUD();

    if (CrosshairTexture.IsValid() && Canvas)
    {
        // Find the center of our canvas.
        FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

        // Offset by half of the texture's dimensions so that the center of the texture aligns with the center of the Canvas.
        FVector2D CrossHairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));

        // Draw the crosshair at the centerpoint.
        FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
        TileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(TileItem);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLHUD::UpdateCrossHair(UTexture2D* CrosshairTextureExt)
{
    CrosshairTexture = CrosshairTextureExt;
}