//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLPowerupQuadDamage.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "QLUtility.h"
#include "QLCharacter.h"
#include "TimerManager.h"
#include "QLUmgUserWidget.h"
#include "QLPlayerController.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLPowerupQuadDamage::AQLPowerupQuadDamage()
{
     DamageMultiplier = 4.0f;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupQuadDamage::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupQuadDamage::OnEffectEnd()
{
    // clean up
    if (Beneficiary.IsValid())
    {
        Beneficiary->SetDamageMultiplier(1.0f);
        Beneficiary->StopGlow();
    }

    Super::OnEffectEnd();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupQuadDamage::UpdateProgressOnUMG()
{
    Super::UpdateProgressOnUMG();

    if (Beneficiary.IsValid())
    {
        auto* PlayerController = Cast<AQLPlayerController>(Beneficiary->GetController());
        if (PlayerController)
        {
            auto* UMG = PlayerController->GetUMG();
            if (UMG)
            {
                UMG->UpdateQuadDamageProgress(ProgressPercent);
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupQuadDamage::PowerUpPlayer()
{
    Beneficiary->SetDamageMultiplier(DamageMultiplier);
    Beneficiary->StartGlow(GlowColor);
}