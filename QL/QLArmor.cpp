//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLArmor.h"
#include "QLCharacter.h"
#include "Components/SphereComponent.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLArmor::AQLArmor()
{
    ArmorIncrement = 10.0f;
    RespawnInterval = 15.0f;
    bCanBeRespawned = false;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLArmor::OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // add health to player
    if (OtherActor)
    {
        AQLCharacter* Character = Cast<AQLCharacter>(OtherActor);
        if (Character && !Character->GetIsBot())
        {
            if (Character->GetArmor() < Character->GetMaxArmor())
            {
                Character->AddArmor(ArmorIncrement);

                PlaySoundFireAndForget("PickUp");

                SetActorEnableCollision(false);
                SetActorHiddenInGame(true);

                if (bCanBeRespawned)
                {
                    // until the next respawn
                    GetWorldTimerManager().SetTimer(RespawnTimerHandle,
                        this,
                        &AQLArmor::Respawn,
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
void AQLArmor::Respawn()
{
    Destroy();
    GetWorld()->SpawnActor<AQLArmor>(GetClass(), GetActorLocation(), FRotator::ZeroRotator);
}
