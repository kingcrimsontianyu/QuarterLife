//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "QLMovementStyle.h"
#include "Engine/DataAsset.h"
#include "QLMovementParameterQuake.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS(Blueprintable)
class QL_API UQLMovementParameterQuake : public UObject
{
	GENERATED_BODY()

public:
    UQLMovementParameterQuake();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property|Common")
    float MaxWalkSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property|Common")
    float MaxAcceleration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float AirControl;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property|Common")
    //float AccelerationConstant;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property|Common")
    //float SpeedLimitAlongAcceleration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float GroundAccelerationMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float AirAccelerationMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property|Common")
    float SpeedUpperLimit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property|Common")
    float NumOfJumpRequestToleranceTimeInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property|VanillaQuake3")
    float NumOfTrailingTimeInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property|VanillaQuake3")
    float BrakingDecelerationChainStrafeJump;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property|Common")
    float BrakingDecelerationWalking;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property|Common")
    float PenaltyScaleFactorForHoldingJumpButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property|Common")
    float PenaltyScaleFactorForUnchainedStrafeJump;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property|Common")
    float JumpZVelocity;
};
