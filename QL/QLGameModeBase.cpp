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

//------------------------------------------------------------
//------------------------------------------------------------
AQLGameModeBase::AQLGameModeBase() :
Super()
{
    DefaultPawnClass = AQLCharacter::StaticClass();
}