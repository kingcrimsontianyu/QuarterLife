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
#include "GameFramework/CharacterMovementComponent.h"
#include "QLMoveComponentQuake.generated.h"

class UQLMovementParameterQuake;
class AQLCharacter;
//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API UQLMoveComponentQuake : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    UQLMoveComponentQuake();

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

    void SetMovementParameter(UQLMovementParameterQuake* DataAssetQuake);

    virtual void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;

    virtual void QueueJump();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void PostInitProperties() override;

    // If the player has already landed for a frame, and breaking may be applied.
    bool bFallingLastFrame;

    // The multiplier for acceleration when on ground.
    UPROPERTY()
    float GroundAccelerationMultiplier;

    // The multiplier for acceleration when in air.
    UPROPERTY()
    float AirAccelerationMultiplier;

    UPROPERTY()
    float SpeedUpperLimit;

    UPROPERTY()
    int NumOfJumpRequestToleranceFrames;

    // in the last k frames, if the jump button has been pressed
    bool bHasJumpRequested;

    int FirstElementIndex;

    TArray<bool> HasJumpRequestedList;

    FVector InputVectorCached;

    FVector AccelerationCached;

    TWeakObjectPtr<AQLCharacter> MyCharacter;
};
