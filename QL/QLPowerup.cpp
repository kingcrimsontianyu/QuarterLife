//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLPowerup.h"
#include "QLPlayerController.h"
#include "QLUmgUserWidget.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLPowerup::AQLPowerup()
{
    bCanBeRespawned = true;
    RespawnInterval = 120.0f;
    EffectDuration = 30.0f;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerup::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerup::UpdateProgressOnUMG()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLPowerup::GetProgressPercent()
{
    return ProgressPercent;
}