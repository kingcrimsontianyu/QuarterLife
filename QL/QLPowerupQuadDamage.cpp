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
#include "QLUmgFirstPerson.h"
#include "QLPlayerController.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLPowerupQuadDamage::AQLPowerupQuadDamage()
{
    PowerupName = FName(TEXT("QuadDamage"));
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
    Super::OnEffectEnd();

    // clean up
    if (Beneficiary.IsValid())
    {
        Beneficiary->SetDamageMultiplier(1.0f);
        Beneficiary->StopGlow();

        // reset the weak pointer
        Beneficiary.Reset();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupQuadDamage::UpdateProgressOnUMGInternal(const float Value)
{
    if (Beneficiary.IsValid())
    {
        auto* PlayerController = Cast<AQLPlayerController>(Beneficiary->GetController());
        if (PlayerController)
        {
            auto* UMG = PlayerController->GetUMG();
            if (UMG)
            {
                UMG->UpdateQuadDamageProgress(Value);
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupQuadDamage::UpdateProgressOnUMG()
{
    Super::UpdateProgressOnUMG();

    UpdateProgressOnUMGInternal(ProgressPercent);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupQuadDamage::SetUMGVisibility(const bool bFlag)
{
    if (Beneficiary.IsValid())
    {
        auto* PlayerController = Cast<AQLPlayerController>(Beneficiary->GetController());
        if (PlayerController)
        {
            auto* UMG = PlayerController->GetUMG();
            if (UMG)
            {
                UMG->SetQuadDamageProgressVisibility(bFlag);
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupQuadDamage::PowerUpPlayer()
{
    Beneficiary->SetDamageMultiplier(DamageMultiplier);
    Beneficiary->StartGlow();
}