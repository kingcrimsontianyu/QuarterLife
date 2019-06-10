//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLGameModeBase.h"
#include "QLCharacter.h"
#include "QLHUD.h"
#include "QLPlayerController.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLGameModeBase::AQLGameModeBase() :
Super()
{
    DefaultPawnClass = AQLCharacter::StaticClass();

    HUDClass = AQLHUD::StaticClass();

    PlayerControllerClass = AQLPlayerController::StaticClass();
}