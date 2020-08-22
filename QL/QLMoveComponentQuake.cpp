//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------

#include "QLMoveComponentQuake.h"
#include "GameFramework/Character.h"
#include "QLUtility.h"
#include "QLMovementParameterQuake.h"
#include "QLCharacter.h"
#include <limits>
#include "Misc/App.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLMoveComponentQuake::UQLMoveComponentQuake()
{
    bFallingLastFrame = false;
    bHasJumpPressed = false;
    bHasJumpReleased = false;
    bJustlanded = false;
    trailingFrameCounter = std::numeric_limits<uint64>::max();

    // according to UE4 source code comment, 1.0f would be more appropriate than the default 2.0f in the engine.
    BrakingFrictionFactor = 1.0f;

    MovementStyle = EQLMovementStyle::Default;
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLMoveComponentQuake::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLMoveComponentQuake::PostInitProperties()
{
    Super::PostInitProperties();
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLMoveComponentQuake::SetMovementParameter(UQLMovementParameterQuake* MovementParameterQuake)
{
    MaxWalkSpeed = MovementParameterQuake->MaxWalkSpeed;
    MaxAcceleration = MovementParameterQuake->MaxAcceleration;
    AirControl = MovementParameterQuake->AirControl;
    GroundAccelerationMultiplier = MovementParameterQuake->GroundAccelerationMultiplier;
    AirAccelerationMultiplier = MovementParameterQuake->AirAccelerationMultiplier;
    SpeedUpperLimit = MovementParameterQuake->SpeedUpperLimit;

    NumOfJumpRequestToleranceTimeInterval = MovementParameterQuake->NumOfJumpRequestToleranceTimeInterval;
    NumOfJumpRequestToleranceFrames = int(1.0f / FApp::GetDeltaTime() * NumOfJumpRequestToleranceTimeInterval);

    NumOfTrailingTimeInterval = MovementParameterQuake->NumOfTrailingTimeInterval;
    NumOfTrailingFrame = int(1.0f / FApp::GetDeltaTime() * NumOfTrailingTimeInterval);

    BrakingDecelerationWalking = MovementParameterQuake->BrakingDecelerationWalking;

    PenaltyScaleFactorForHoldingJumpButton = MovementParameterQuake->PenaltyScaleFactorForHoldingJumpButton;

    PenaltyScaleFactorForUnchainedStrafeJump = MovementParameterQuake->PenaltyScaleFactorForUnchainedStrafeJump;
    PenaltyForUnchainedStrafeJumpReductionPerFrame = (1.0f - PenaltyScaleFactorForUnchainedStrafeJump) / NumOfTrailingFrame;

    JumpZVelocity = MovementParameterQuake->JumpZVelocity;

    HasJumpPressedList.Init(false, NumOfJumpRequestToleranceFrames);
    FirstElementIndexForJumpPressed = 0;

    HasJumpReleasedList.Init(false, NumOfJumpRequestToleranceFrames);
    FirstElementIndexForJumpReleased = 0;
}

//------------------------------------------------------------
// enum ENetRole
// {
//     ROLE_None,
//     ROLE_SimulatedProxy,
//     ROLE_AutonomousProxy,
//     ROLE_Authority,
//     ROLE_MAX,
// }
// For the single player game, by default ENetRole::ROLE_Authority is used
//------------------------------------------------------------
void UQLMoveComponentQuake::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    if (MovementStyle == EQLMovementStyle::Default)
    {
        Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    }
    else
    {
        InputVectorCached = ConsumeInputVector();

        MyCharacter = Cast<AQLCharacter>(CharacterOwner);

        CheckJumpInfo();

        if (!HasValidData() || ShouldSkipUpdate(DeltaTime))
        {
            return;
        }

        // Super tick may destroy/invalidate CharacterOwner or UpdatedComponent, so we need to re-check.
        if (!HasValidData())
        {
            return;
        }

        // See if we fell out of the world.
        const bool bIsSimulatingPhysics = UpdatedComponent->IsSimulatingPhysics();
        if ((!bCheatFlying || bIsSimulatingPhysics) && !CharacterOwner->CheckStillInWorld())
        {
            return;
        }

        // We don't update if simulating physics (eg ragdolls).
        if (bIsSimulatingPhysics)
        {
            ClearAccumulatedForces();
            return;
        }

        AvoidanceLockTimer -= DeltaTime;

        // Allow root motion to move characters that have no controller.
        if (CharacterOwner->IsLocallyControlled() || (!CharacterOwner->Controller && bRunPhysicsWithNoController) || (!CharacterOwner->Controller && CharacterOwner->IsPlayingRootMotion()))
        {
            {
                // We need to check the jump state before adjusting input acceleration, to minimize latency
                // and to make sure acceleration respects our potentially new falling state.
                CharacterOwner->CheckJumpInput(DeltaTime);

                // apply input to acceleration
                // --- note 1:
                // InputVectorCached = ConsumeInputVector(): is the unnormalized input vector in the world coordinate.
                // In the local coordinate, pressing W/S results in X=1/-1, and pressing A/D results in Y=-1/1.
                // So the unnormalized local input vector has 3 discrete possible length: 0, 1, sqrt(2).
                // InputVectorCached is a transform of such vector into the world coordinate.
                //
                // --- note 2:
                // Acceleration = ScaleInputAcceleration(ConstrainInputAcceleration(InputVectorCached)): is the unnormalized
                // acceleration vector in the world coordinate.
                // If MaxAcceleration is set to 1000, then Acceleration here has a length of 1000 and has the same direction
                // with InputVectorCached.
                Acceleration = ScaleInputAcceleration(ConstrainInputAcceleration(InputVectorCached));
                AccelerationCached = Acceleration;

                AnalogInputModifier = ComputeAnalogInputModifier();
            }

            PerformMovement(DeltaTime);
        }

        if (bUseRVOAvoidance)
        {
            UpdateDefaultAvoidance();
        }

        if (bEnablePhysicsInteraction)
        {
            ApplyDownwardForce(DeltaTime);
            ApplyRepulsionForce(DeltaTime);
        }

        PrepareForNextFrame();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLMoveComponentQuake::CheckJumpInfo()
{
    if (MyCharacter.IsValid())
    {
        if (HasJumpPressedList.Num() > 0)
        {
            HasJumpPressedList[FirstElementIndexForJumpPressed] = MyCharacter->IsJumpButtonDown();

            bHasJumpPressed = false;
            for (auto&& item : HasJumpPressedList)
            {
                if (item)
                {
                    bHasJumpPressed = true;
                    break;
                }
            }
        }

        if (HasJumpReleasedList.Num() > 0)
        {
            HasJumpReleasedList[FirstElementIndexForJumpReleased] = !MyCharacter->IsJumpButtonDown();

            bHasJumpReleased = false;
            for (auto&& item : HasJumpReleasedList)
            {
                if (item)
                {
                    bHasJumpReleased = true;
                    break;
                }
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLMoveComponentQuake::PrepareForNextFrame()
{
    // immediately make the player jump once they reach the ground
    if (IsMovingOnGround() && // in current frame the player is on the ground
        bFallingLastFrame && // in last frame the player is falling
        bHasJumpPressed) // the player has recently pressed jump button
    {
        // jump in the next frame
        // the movement mode will be set to falling in the next frame
        DoJump(true);

        bJustlanded = true;
    }
    else
    {
        bJustlanded = false;
    }

    // play the "huh" sound only if the strafe jump is not successfully chained
    // and regular jump is just performed
    if (IsFalling() && // in current frame the player just jumps (in falling state)
        !bFallingLastFrame && // in last frame the player is on the ground
        bHasJumpPressed) // the player has recently pressed jump button
    {
        if (MyCharacter.IsValid())
        {
            MyCharacter->PlaySoundFireAndForget(FName(TEXT("QuakeJump")));
        }
    }

    bFallingLastFrame = !IsMovingOnGround();

    ++FirstElementIndexForJumpPressed;
    if (FirstElementIndexForJumpPressed >= NumOfJumpRequestToleranceFrames)
    {
        FirstElementIndexForJumpPressed = 0;
    }

    ++FirstElementIndexForJumpReleased;
    if (FirstElementIndexForJumpReleased >= NumOfJumpRequestToleranceFrames)
    {
        FirstElementIndexForJumpReleased = 0;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLMoveComponentQuake::CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration)
{
    float moveForwardInputValue = MyCharacter->GetMoveForwardInputValue();

    if (MovementStyle == EQLMovementStyle::Default)
    {
        Super::CalcVelocity(DeltaTime, Friction, bFluid, BrakingDeceleration);
    }
    else
    {
        // Do not update velocity when using root motion or when SimulatedProxy and not simulating root motion - SimulatedProxy are repped their Velocity
        if (!HasValidData() || HasAnimRootMotion() || DeltaTime < MIN_TICK_TIME || (CharacterOwner && CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy && !bWasSimulatingRootMotion))
        {
            return;
        }

        Friction = FMath::Max(0.0f, Friction);
        const float MaxAccel = GetMaxAcceleration();
        float MaxSpeed = GetMaxSpeed();

        // Check if path following requested movement
        bool bZeroRequestedAcceleration = true;
        FVector RequestedAcceleration = FVector::ZeroVector;
        float RequestedSpeed = 0.0f;
        if (ApplyRequestedMove(DeltaTime, MaxAccel, MaxSpeed, Friction, BrakingDeceleration, RequestedAcceleration, RequestedSpeed))
        {
            bZeroRequestedAcceleration = false;
        }

        // todo: bForceMaxAccel always evaluates to false ?!
        if (bForceMaxAccel)
        {
            // Force acceleration at full speed.
            // In consideration order for direction: Acceleration, then Velocity, then Pawn's rotation.
            if (Acceleration.SizeSquared() > SMALL_NUMBER)
            {
                Acceleration = Acceleration.GetSafeNormal() * MaxAccel;
            }
            else
            {
                Acceleration = MaxAccel * (Velocity.SizeSquared() < SMALL_NUMBER ? UpdatedComponent->GetForwardVector() : Velocity.GetSafeNormal());
            }

            AnalogInputModifier = 1.0f;
        }

        const bool bZeroAcceleration = AccelerationCached.IsZero();
        const bool bVelocityOverMax = IsExceedingMaxSpeed(MaxSpeed);

        if (IsMovingOnGround() && bFallingLastFrame)
        {
            bJustlanded = true;
        }

        if (bJustlanded)
        {
            trailingFrameCounter = 0ULL;
            VelocityCached = Velocity;
            PenaltyForUnchainedStrafeJumpCurrent = 1.0f;
        }
        else
        {
            if (trailingFrameCounter < std::numeric_limits<uint64>::max())
            {
                ++trailingFrameCounter;
            }
        }

        // case 1: the player is on the ground in both the current frame and last frame
        if (IsMovingOnGround() &&
            !bFallingLastFrame)
        {
            // sub-case 1: if there is no input
            if (bZeroAcceleration)
            {
                const FVector OldVelocity = Velocity;

                const float ActualBrakingFriction = (bUseSeparateBrakingFriction ? BrakingFriction : Friction);
                // BrakingDeceleration is passed as an argument via CalcVelocity().
                // If the player is considered walking on the ground, BrakingDeceleration is BrakingDecelerationWalking
                ApplyVelocityBraking(DeltaTime, ActualBrakingFriction, BrakingDeceleration);
            }
            // sub-case 2: input exists
            else
            {
                if (Velocity.Size() < MaxSpeed ||
                    trailingFrameCounter > NumOfTrailingFrame)
                {
                    // sub-sub-case 2: the player is normally walking
                    // a large acceleration is applied, followed by a speed cap,
                    // so that the player can change from stationary to moving in a short period of time
                    Velocity += AccelerationCached * GroundAccelerationMultiplier * DeltaTime;
                    Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);
                }
                else
                {
                    PenaltyForUnchainedStrafeJumpCurrent -= PenaltyForUnchainedStrafeJumpReductionPerFrame;
                    Velocity = MyCharacter->GetActorForwardVector() * VelocityCached.Size() * PenaltyForUnchainedStrafeJumpCurrent;
                }
            }
        }
        // case 2: air strafe
        // This includes the general case for mid air, and also includes a special one-frame case where
        // the player is on the ground in the current frame (but still rendered in the
        // falling state due to calling DoJump() in the last frame), but falling in the last frame
        else if (bFallingLastFrame || // in last frame the player is falling
                                      // in the current frame the player may or may not be on the ground
                 IsFalling()) // in the current frame the player is falling as well
        {
            if (MovementStyle == EQLMovementStyle::QuakeVanilla)
            {
                HandleAirStrafeForVanilla(MaxSpeed, DeltaTime, Friction, BrakingDeceleration);
            }
            else if (MovementStyle == EQLMovementStyle::QuakeCPMA)
            {
                HandleAirStrafeForCPMA(MaxSpeed, DeltaTime, Friction, BrakingDeceleration);
            }
        }

        // Apply fluid friction
        if (bFluid)
        {
            Velocity = Velocity * (1.0f - FMath::Min(Friction * DeltaTime, 1.0f));
        }

        // todo: bZeroRequestedAcceleration always evaluates to true,
        // i.e. !bZeroRequestedAcceleration always evaluates to false
        // Apply additional requested acceleration
        if (!bZeroRequestedAcceleration)
        {
            Velocity += RequestedAcceleration * DeltaTime;
        }

        // impose final speed cap
        if (Velocity.Size2D() > SpeedUpperLimit)
        {
            Velocity = Velocity.GetClampedToMaxSize2D(SpeedUpperLimit);
        }

        if (bUseRVOAvoidance)
        {
            CalcAvoidanceVelocity(DeltaTime);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLMoveComponentQuake::HandleAirStrafeForVanilla(float MaxSpeed, float DeltaTime, float Friction, float BrakingDeceleration)
{
    if (MyCharacter.IsValid())
    {
        float moveForwardInputValue = MyCharacter->GetMoveForwardInputValue();
        float moveRightInputValue = MyCharacter->GetMoveRightInputValue();

        // Disallow the use of CPMA movement style. In other words, to gain acceleration,
        // players must hold forward key while strafe jumping.
        // Otherwise deceleration applies.
        if (moveForwardInputValue == 0.0f && moveRightInputValue != 0.0f)
        {
            // to do: figure out the relation between braking and friction
            const float ActualBrakingFriction = (bUseSeparateBrakingFriction ? BrakingFriction : Friction);
            ApplyVelocityBraking(DeltaTime, ActualBrakingFriction, 100.0f);
        }
        // if no input key is pressed, lose speed more quickly
        else if (moveForwardInputValue == 0.0f && moveRightInputValue == 0.0f)
        {
            // to do: more elegant specification of friction and braking
            const float ActualBrakingFriction = (bUseSeparateBrakingFriction ? BrakingFriction : Friction);
            ApplyVelocityBraking(DeltaTime, ActualBrakingFriction, 600.0f);
        }
        else
        {
            // theory in a nutshell:
            // current speed: Velocity (v_3d)
            // acceleration direction: AccelDirection (d_3d)
            // acceleration length: m = AccelerationCached.Size() * AirAccelerationMultiplier
            // proposed speed increase along d_3d: ProposedSpeedIncrease = m * delta_T (A)
            // projection of current speed onto acceleration direction: p = DotProduct(v_3d, d_3d)
            // max speed increase allowed along d_3d: MaxSpeedIncrease = MaxSpeed - p (K)
            // if A <= K, new speed: v_3d + d_3d * A
            // if A > K, new speed: v_3d + d_3d * K

            const FVector AccelDirection = AccelerationCached.GetSafeNormal2D();

            const float SpeedProjection = Velocity.X * AccelDirection.X + Velocity.Y * AccelDirection.Y;

            const float MaxSpeedIncrease = MaxSpeed - SpeedProjection;
            if (MaxSpeedIncrease > 0.0f)
            {
                float ProposedSpeedIncrease = AccelerationCached.Size() * AirAccelerationMultiplier * DeltaTime;
                float ActualSpeedIncrease;

                if (ProposedSpeedIncrease > MaxSpeedIncrease)
                {
                    ActualSpeedIncrease = MaxSpeedIncrease;
                }
                else
                {
                    ActualSpeedIncrease = ProposedSpeedIncrease;
                }

                // if the player keeps pressing the jump button to strafe jump,
                // as a punishment, the acceleration is reduced
                if (bHasJumpPressed && !bHasJumpReleased)
                {
                    ActualSpeedIncrease *= PenaltyScaleFactorForHoldingJumpButton;
                }

                // Apply acceleration
                FVector CurrentAcceleration = ActualSpeedIncrease * AccelDirection;

                Velocity += CurrentAcceleration;
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLMoveComponentQuake::HandleAirStrafeForCPMA(float MaxSpeed, float DeltaTime, float Friction, float BrakingDeceleration)
{
    if (MyCharacter.IsValid())
    {
        float moveForwardInputValue = MyCharacter->GetMoveForwardInputValue();
        float moveRightInputValue = MyCharacter->GetMoveRightInputValue();

        // Disallow the use of VQ3 movement style. In other words, to gain acceleration,
        // players must NOT hold forward key while strafe jumping.
        // Otherwise deceleration applies.
        if (moveForwardInputValue != 0.0f && moveRightInputValue != 0.0f)
        {
            // to do: figure out the relation between braking and friction
            const float ActualBrakingFriction = (bUseSeparateBrakingFriction ? BrakingFriction : Friction);
            ApplyVelocityBraking(DeltaTime, ActualBrakingFriction, 100.0f);
        }
        // if no input key is pressed, lose speed more quickly
        else if (moveForwardInputValue == 0.0f && moveRightInputValue == 0.0f)
        {
            // to do: more elegant specification of friction and braking
            const float ActualBrakingFriction = (bUseSeparateBrakingFriction ? BrakingFriction : Friction);
            ApplyVelocityBraking(DeltaTime, ActualBrakingFriction, 600.0f);
        }
        else
        {
            QLUtility::Log(FString("CPMA boost"));

            const FVector AccelDirection = AccelerationCached.GetSafeNormal2D();

            const float SpeedProjection = Velocity.X * AccelDirection.X + Velocity.Y * AccelDirection.Y;

            const float MaxSpeedIncrease = MaxSpeed - SpeedProjection;
            if (MaxSpeedIncrease > 0.0f)
            {
                float ProposedSpeedIncrease = AccelerationCached.Size() * AirAccelerationMultiplier * DeltaTime;
                float ActualSpeedIncrease;

                if (ProposedSpeedIncrease > MaxSpeedIncrease)
                {
                    ActualSpeedIncrease = MaxSpeedIncrease;
                }
                else
                {
                    ActualSpeedIncrease = ProposedSpeedIncrease;
                }

                // if the player keeps pressing the jump button to strafe jump,
                // as a punishment, the acceleration is reduced
                if (bHasJumpPressed && !bHasJumpReleased)
                {
                    ActualSpeedIncrease *= PenaltyScaleFactorForHoldingJumpButton;
                }

                // Apply acceleration
                FVector CurrentAcceleration = ActualSpeedIncrease * AccelDirection;

                Velocity += CurrentAcceleration;
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLMoveComponentQuake::SetMovementStyle(EQLMovementStyle MyStyle)
{
    MovementStyle = MyStyle;
}

//------------------------------------------------------------
//------------------------------------------------------------
EQLMovementStyle UQLMoveComponentQuake::GetMovementStyle()
{
    return MovementStyle;
}