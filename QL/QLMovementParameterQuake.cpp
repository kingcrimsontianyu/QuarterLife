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
    MaxAcceleration = 600.0f;
    AirControl = 0.0f;
    GroundAccelerationMultiplier = 10.0f;
    AirAccelerationMultiplier = 2.0f;
    SpeedUpperLimit = 3000.0f;
    NumOfJumpRequestToleranceFrames = 5;
    BrakingDecelerationWalking = MaxAcceleration;
    PenaltyScaleFactorForHoldingJumpButton = 0.25f;
}