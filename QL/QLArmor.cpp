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
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLArmor::OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AddArmorToPlayer(OtherActor);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLArmor::OnComponentHitImpl(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    AddArmorToPlayer(OtherActor);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLArmor::AddArmorToPlayer(AActor* OtherActor)
{
    if (OtherActor)
    {
        AQLCharacter* Character = Cast<AQLCharacter>(OtherActor);
        if (Character && !Character->QLIsBot())
        {
            if (Character->GetArmor() < Character->GetMaxArmor())
            {
                Character->AddArmor(ArmorIncrement);

                PlaySoundFireAndForget("PickUp");
                Destroy();
            }
        }
    }
}
