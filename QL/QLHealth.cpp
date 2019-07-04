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
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLHealth::OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AddHealthToPlayer(OtherActor);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLHealth::OnComponentHitImpl(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    AddHealthToPlayer(OtherActor);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLHealth::AddHealthToPlayer(AActor* OtherActor)
{
    if (OtherActor)
    {
        AQLCharacter* Character = Cast<AQLCharacter>(OtherActor);
        if (Character && !Character->IsBot())
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