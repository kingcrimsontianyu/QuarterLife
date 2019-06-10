//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLUtility.h"
#include "Engine/Engine.h"

namespace QLUtility
{
    //------------------------------------------------------------
    //------------------------------------------------------------
    void Log(const FString& msg)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void Screen(const FString& msg)
    {
        bool bNewerOnTop = true;
        FVector2D TextScale(1.0f, 1.0f);
        GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Cyan, msg, bNewerOnTop, TextScale);
    }

}

