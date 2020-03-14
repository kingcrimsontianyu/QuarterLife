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

//------------------------------------------------------------
//------------------------------------------------------------
UQLMoveComponentQuake::UQLMoveComponentQuake()
{
    bFallingLastFrame = false;
    bHasJumpRequested = false;

    // according to UE4 source code comment, 1.0f would be more appropriate than the default 2.0f in the engine.
    BrakingFrictionFactor = 1.0f;
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
    NumOfJumpRequestToleranceFrames = MovementParameterQuake->NumOfJumpRequestToleranceFrames;
    BrakingDecelerationWalking = MovementParameterQuake->BrakingDecelerationWalking;

    HasJumpRequestedList.Init(false, NumOfJumpRequestToleranceFrames);
    FirstElementIndex = 0;
}

//------------------------------------------------------------
//enum ENetRole
//{
//    ROLE_None,
//    ROLE_SimulatedProxy,
//    ROLE_AutonomousProxy,
//    ROLE_Authority,
//    ROLE_MAX,
//}
// For the single player game, by default ENetRole::ROLE_Authority is used
//------------------------------------------------------------
void UQLMoveComponentQuake::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    InputVectorCached = ConsumeInputVector();

    MyCharacter = Cast<AQLCharacter>(CharacterOwner);

    if (MyCharacter.IsValid())
    {
        if (HasJumpRequestedList.Num() > 0)
        {
            HasJumpRequestedList[FirstElementIndex] = MyCharacter->IsJumpButtonDown();

            bHasJumpRequested = false;
            for (auto&& item : HasJumpRequestedList)
            {
                if (item)
                {
                    bHasJumpRequested = true;
                    break;
                }
            }
        }
    }

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
            Acceleration = ScaleInputAcceleration(ConstrainInputAcceleration(InputVectorCached));
            AccelerationCached = Acceleration;

            AnalogInputModifier = ComputeAnalogInputModifier();
        }

        PerformMovement(DeltaTime);

        if (IsMovingOnGround() &&
            bFallingLastFrame &&
            bHasJumpRequested)
        {
            if (MyCharacter.IsValid())
            {
                MyCharacter->PlaySoundFireAndForget(FName(TEXT("QuakeJump")));
            }
            DoJump(true);
        }
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

    bFallingLastFrame = !IsMovingOnGround();

    ++FirstElementIndex;
    if (FirstElementIndex >= NumOfJumpRequestToleranceFrames)
    {
        FirstElementIndex = 0;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLMoveComponentQuake::CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration)
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

    // apply braking
    const bool bZeroAcceleration = AccelerationCached.IsZero();
    const bool bVelocityOverMax = IsExceedingMaxSpeed(MaxSpeed);

    // brake is applicable only when
    // --- there is no input
    // --- the player is on the ground in the current frame
    // --- the player is not falling in the last frame
    if (bZeroAcceleration &&
        IsMovingOnGround() &&
        !bFallingLastFrame)
    {
        const FVector OldVelocity = Velocity;

        const float ActualBrakingFriction = (bUseSeparateBrakingFriction ? BrakingFriction : Friction);
        ApplyVelocityBraking(DeltaTime, ActualBrakingFriction, BrakingDeceleration);

        //// Don't allow braking to lower us below max speed if we started above it.
        //if (bVelocityOverMax && Velocity.SizeSquared() < FMath::Square(MaxSpeed) && FVector::DotProduct(Acceleration, OldVelocity) > 0.0f)
        //{
        //    Velocity = OldVelocity.GetSafeNormal() * MaxSpeed;
        //}
    }

    // Apply fluid friction
    if (bFluid)
    {
        Velocity = Velocity * (1.0f - FMath::Min(Friction * DeltaTime, 1.0f));
    }

    // Apply input acceleration, the section of paramount importance to advanced movement!
    if (!bZeroAcceleration)
    {
        if (IsMovingOnGround() &&
            !bFallingLastFrame)
        {
            Velocity += AccelerationCached * GroundAccelerationMultiplier * DeltaTime;
            Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);
        }
        else if(bFallingLastFrame || IsFalling())
        {
            const FVector AccelDirection = AccelerationCached.GetSafeNormal2D();

            const float SpeedProjection = Velocity.X * AccelDirection.X + Velocity.Y * AccelDirection.Y;

            const float AddSpeed = MaxSpeed - SpeedProjection;
            if (AddSpeed > 0.0f)
            {
                float AnotherAddSpeedCandidate = AccelerationCached.Size() * AirAccelerationMultiplier * DeltaTime;

                if (AnotherAddSpeedCandidate > AddSpeed)
                {
                    AnotherAddSpeedCandidate = AddSpeed;
                }

                // Apply acceleration
                FVector CurrentAcceleration = AnotherAddSpeedCandidate * AccelDirection;

                Velocity += CurrentAcceleration;
            }
        }
    }

    // todo: bZeroRequestedAcceleration always evaluates to true,
    // i.e. !bZeroRequestedAcceleration always evaluates to false
    // Apply additional requested acceleration
    if (!bZeroRequestedAcceleration)
    {
        Velocity += RequestedAcceleration * DeltaTime;
    }

    if (bUseRVOAvoidance)
    {
        CalcAvoidanceVelocity(DeltaTime);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLMoveComponentQuake::QueueJump()
{
}