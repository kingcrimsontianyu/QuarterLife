//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLMovementParameterQuake.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLMovementParameterQuake::UQLMovementParameterQuake()
{
    MaxWalkSpeed = 600.0f;
    MaxAcceleration = 1000.0f;
    AirControl = 0.3f;
    GroundAccelerationMultiplier = 10.0f;
    AirAccelerationMultiplier = 1.0f;
}