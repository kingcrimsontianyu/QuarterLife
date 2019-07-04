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

//------------------------------------------------------------
//------------------------------------------------------------
AQLHealth::AQLHealth()
{
    HealthIncrement = 10.0f;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLHealth::OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        AQLCharacter* Character = Cast<AQLCharacter>(OtherActor);
        if (Character)
        {
            if (Character->GetHealth() < Character->GetMaxHealth())
            {
                Character->AddHealth(HealthIncrement);

                PlaySoundFireAndForget("PickUp");
                Destroy();
            }
        }
    }
}

