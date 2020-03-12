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

//#include "GameFramework/CharacterMovementComponent.h"
//#include "EngineStats.h"
//#include "Components/PrimitiveComponent.h"
//#include "AI/NavigationSystemBase.h"
//#include "AI/Navigation/NavigationDataInterface.h"
//#include "UObject/Package.h"
//#include "GameFramework/PlayerController.h"
//#include "GameFramework/PhysicsVolume.h"
//#include "Components/SkeletalMeshComponent.h"
//#include "Engine/NetDriver.h"
//#include "DrawDebugHelpers.h"
//#include "GameFramework/GameNetworkManager.h"
//#include "GameFramework/Character.h"
//#include "Components/CapsuleComponent.h"
//#include "GameFramework/GameStateBase.h"
//#include "Engine/Canvas.h"
//#include "AI/Navigation/PathFollowingAgentInterface.h"
//#include "AI/Navigation/AvoidanceManager.h"
//#include "Components/BrushComponent.h"
//#include "Misc/App.h"
//
//#include "Engine/DemoNetDriver.h"
//#include "Engine/NetworkObjectList.h"
//
//#include "Net/PerfCountersHelpers.h"
//#include "ProfilingDebugging/CsvProfiler.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLMoveComponentQuake::UQLMoveComponentQuake()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLMoveComponentQuake::BeginPlay()
{
    Super::BeginPlay();

    MaxWalkSpeed = 600.0f;
    AirControl = 0.3f;
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLMoveComponentQuake::PostInitProperties()
{
    Super::PostInitProperties();
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
    const FVector InputVector = ConsumeInputVector();
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
            Acceleration = ScaleInputAcceleration(ConstrainInputAcceleration(InputVector));
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
}