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
    if (OtherActor)
    {
        AQLCharacter* Character = Cast<AQLCharacter>(OtherActor);
        if (Character)
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
