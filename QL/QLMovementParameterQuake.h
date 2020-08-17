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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float MaxWalkSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float MaxAcceleration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float AirControl;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float AccelerationConstant;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float SpeedLimitAlongAcceleration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float GroundAccelerationMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float AirAccelerationMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float SpeedUpperLimit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    int NumOfJumpRequestToleranceFrames;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float BrakingDecelerationWalking;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float PenaltyScaleFactorForHoldingJumpButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float JumpZVelocity;
};
