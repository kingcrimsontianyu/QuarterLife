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

    void SetMovementStyle(EQLMovementStyle MyStyle);

    EQLMovementStyle GetMovementStyle();

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

    void SetMovementParameter(UQLMovementParameterQuake* DataAssetQuake);

    virtual void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;

    virtual void QueueJump();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void PostInitProperties() override;

    // check if jump button has been pressed and released in the past k frames, including the current frame
    void CheckJumpInfo();

    void PrepareForNextFrame();

    void HandleAirStrafeForVanilla(float MaxSpeed, float DeltaTime, float Friction, float BrakingDeceleration);

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

    UPROPERTY()
    float PenaltyScaleFactorForHoldingJumpButton;

    // in the last k frames, if the jump button has been pressed
    bool bHasJumpPressed;

    int FirstElementIndexForJumpPressed;

    TArray<bool> HasJumpPressedList;

    // in the last k frames, if the jump button has been released
    bool bHasJumpReleased;

    int FirstElementIndexForJumpReleased;

    TArray<bool> HasJumpReleasedList;

    FVector InputVectorCached;

    FVector AccelerationCached;

    TWeakObjectPtr<AQLCharacter> MyCharacter;

    EQLMovementStyle MovementStyle;
};
