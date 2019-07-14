//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLHealth.h"
#include "QLCharacter.h"
#include "QLUtility.h"
#include "Components/SphereComponent.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLHealth::AQLHealth()
{
    HealthIncrement = 10.0f;
    RespawnInterval = 15.0f;
    bCanBeRespawned = false;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLHealth::OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // add health to player
    if (OtherActor)
    {
        AQLCharacter* Character = Cast<AQLCharacter>(OtherActor);
        if (Character && !Character->GetIsBot())
        {
            if (Character->GetHealth() < Character->GetMaxHealth())
            {
                Character->AddHealth(HealthIncrement);

                PlaySoundFireAndForget("PickUp");

                SetActorEnableCollision(false);
                SetActorHiddenInGame(true);

                if (bCanBeRespawned)
                {
                    // until the next respawn
                    GetWorldTimerManager().SetTimer(RespawnTimerHandle,
                        this,
                        &AQLHealth::Respawn,
                        1.0f, // time interval in second
                        false, // loop
                        RespawnInterval); // delay in second
                }
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLHealth::Respawn()
{
    Destroy();
    GetWorld()->SpawnActor<AQLHealth>(GetClass(), GetActorLocation(), FRotator::ZeroRotator);
}