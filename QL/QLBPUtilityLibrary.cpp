//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLBPUtilityLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "QLCharacter.h"
#include "QLAbilityManager.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"

//------------------------------------------------------------
//------------------------------------------------------------
void UQLBPUtilityLibrary::InitializePlayer(const EQLAbility InitialAbility,
    const EQLWeapon InitialWeapon,
    const UObject* WorldContextObj,
    const AActor* NearActor,
    const AActor* FarActor)
{
    if (!WorldContextObj)
    {
        return;
    }

    ACharacter* MyACharacter = UGameplayStatics::GetPlayerCharacter(WorldContextObj, 0);

    if (MyACharacter)
    {
        AQLCharacter* MyQLCharacter = Cast<AQLCharacter>(MyACharacter);

        if (MyQLCharacter)
        {
            MyQLCharacter->EquipAll();
            MyQLCharacter->SetCurrentAbility(InitialAbility);
            MyQLCharacter->SetCurrentWeapon(InitialWeapon);

            if (NearActor != nullptr && FarActor != nullptr)
            {
                // need to check if near actor is really near the player
                // if not, swap near and far actors
                AActor* ActualNearActor = const_cast<AActor*>(NearActor);
                AActor* ActualFarActor = const_cast<AActor*>(FarActor);

                float DistSqrNear = FVector::DistSquared(MyQLCharacter->GetActorLocation(), ActualNearActor->GetActorLocation());
                float DistSqrFar = FVector::DistSquared(MyQLCharacter->GetActorLocation(), ActualFarActor->GetActorLocation());

                if (DistSqrNear > DistSqrFar)
                {
                    AActor* Temp = ActualNearActor;
                    ActualNearActor = ActualFarActor;
                    ActualFarActor = Temp;
                }

                MyQLCharacter->GetAbilityManager()->InitializeAbilityTimeTravel(ActualNearActor, ActualFarActor);
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLBPUtilityLibrary::HandleExponentialHeightFogPerTick(const UObject* WorldContextObj,
    const float InitialFogDensity,
    AExponentialHeightFog* FogActor,
    const float InterpStartDistance,
    const float InterpEndDistance)
{
    if (!WorldContextObj)
    {
        return;
    }

    ACharacter* MyACharacter = UGameplayStatics::GetPlayerCharacter(WorldContextObj, 0);
    if (!MyACharacter)
    {
        return;
    }

    float CurrentDistance = FVector::Distance(MyACharacter->GetActorLocation(),
        FogActor->GetActorLocation());

    if (InterpEndDistance <= InterpStartDistance)
    {
        return;
    }

    // Three cases:
    // (1) distance between player camera and fog actor is less than FogReductionStartDistance, fog density is unchanged,
    // (2) distance greater than FogReductionStartDistance and less than FogReductionEndDistance, fog density is interpolated,
    // (3) distance is greater than FogReductionEndDistance, fog density is 0.
    float CurrentFogDensity = InitialFogDensity;
    if (CurrentDistance >= InterpStartDistance && CurrentDistance <= InterpEndDistance)
    {
        float alpha = (CurrentDistance - InterpStartDistance) / (InterpEndDistance - InterpStartDistance);
        CurrentFogDensity = InitialFogDensity * (1.0f - alpha);
    }
    else if (CurrentDistance > InterpEndDistance)
    {
        CurrentFogDensity = 0.0f;
    }

    FogActor->GetComponent()->SetFogDensity(CurrentFogDensity);
}

