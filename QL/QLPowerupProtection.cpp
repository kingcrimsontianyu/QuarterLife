//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLPowerupProtection.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "QLUtility.h"
#include "QLCharacter.h"
#include "TimerManager.h"
#include "QLUmgUserWidget.h"
#include "QLPlayerController.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLPowerupProtection::AQLPowerupProtection()
{
    ProtectionMultiplier = 0.3f;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupProtection::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupProtection::OnEffectEnd()
{
    // clean up
    if (Beneficiary.IsValid())
    {
        Beneficiary->SetProtectionMultiplier(1.0f);
        Beneficiary->StopGlow();
    }

    Super::OnEffectEnd();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupProtection::UpdateProgressOnUMG()
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
                UMG->UpdateProtectionDamageProgress(ProgressPercent);
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupProtection::PowerUpPlayer()
{
    Beneficiary->SetProtectionMultiplier(ProtectionMultiplier);
    Beneficiary->StartGlow(GlowColor);
}