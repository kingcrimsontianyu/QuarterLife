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

void UQLBPUtilityLibrary::InitializePlayer(const FString& InitialAbility,
    const FString& InitialWeapon,
    const UObject* WorldContextObj,
    const AActor* NearActor,
    const AActor* FarActor)
{
    ACharacter* MyACharacter = UGameplayStatics::GetPlayerCharacter(WorldContextObj, 0);

    if (MyACharacter)
    {
        AQLCharacter* MyQLCharacter = Cast<AQLCharacter>(MyACharacter);

        if (MyQLCharacter)
        {
            MyQLCharacter->EquipAll();
            MyQLCharacter->SetCurrentAbility(FName(InitialAbility));
            MyQLCharacter->SetCurrentWeapon(FName(InitialWeapon));

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

