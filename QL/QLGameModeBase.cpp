//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------


#include "QLGameModeBase.h"
#include "QLCharacter.h"
#include "QLHUD.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLGameModeBase::AQLGameModeBase() :
Super()
{
    DefaultPawnClass = AQLCharacter::StaticClass();

    // use our custom HUD class
    HUDClass = AQLHUD::StaticClass();
}