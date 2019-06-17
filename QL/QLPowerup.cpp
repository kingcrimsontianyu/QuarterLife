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

//------------------------------------------------------------
//------------------------------------------------------------
AQLPowerup::AQLPowerup()
{
    bCanBeRespawned = true;
    RespawnInterval = 120.0f;
    EffectInterval = 30.0f;

    GlowColor = FVector(0.0f, 0.0f, 1.0f);
}