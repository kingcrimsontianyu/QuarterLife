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
    RootSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AQLPowerupQuadDamage::OnComponentBeginOverlapImpl);

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
void AQLPowerupQuadDamage::OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        AQLCharacter* QLCharacter = Cast<AQLCharacter>(OtherActor);
        if (QLCharacter)
        {
            Beneficiary = QLCharacter;

            if (!Beneficiary.IsValid())
            {
                return;
            }

            PlaySoundFireAndForget("PickUp");

            Beneficiary->SetDamageMultiplier(DamageMultiplier);
            Beneficiary->StartGlow(GlowColor);

            Deactivate();

            TimeElapsed = 0.0f;

            // countdown of the next respawn
            GetWorldTimerManager().SetTimer(RespawnTimerHandle,
                this,
                &AQLPowerupQuadDamage::Reactivate,
                1.0f, // time interval in second
                false, // loop
                RespawnInterval); // delay in second

            // take effect immediately
            GetWorldTimerManager().SetTimer(EffectStartTimerHandle,
                this,
                &AQLPowerupQuadDamage::UpdateProgressOnUMG,
                0.1f, // time interval in second
                true, // loop
                0.0f); // delay in second

            GetWorldTimerManager().SetTimer(EffectEndTimerHandle,
                this,
                &AQLPowerupQuadDamage::OnEffectEnd,
                1.0f, // time interval in second
                false, // loop
                EffectDuration); // delay in second
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupQuadDamage::Reactivate()
{
    RootSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AQLPowerupQuadDamage::OnComponentBeginOverlapImpl);

    if (DynamicMaterial.IsValid())
    {
        DynamicMaterial->SetScalarParameterValue("GlowIntensity", 5.0f);
    }

    GetWorldTimerManager().ClearTimer(RespawnTimerHandle);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupQuadDamage::Deactivate()
{
    // temporarily remove delegate until next time quad shows up
    RootSphereComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AQLPowerupQuadDamage::OnComponentBeginOverlapImpl);

    if (DynamicMaterial.IsValid())
    {
        DynamicMaterial->SetScalarParameterValue("GlowIntensity", 0.1f);
    }
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

    // reset the weak pointer
    Beneficiary.Reset();

    GetWorldTimerManager().ClearTimer(EffectEndTimerHandle);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupQuadDamage::UpdateProgressOnUMG()
{
    TimeElapsed += 0.1f;
    ProgressPercent = TimeElapsed / EffectDuration;
    float DisplayedProgressPercent = 1.0f - ProgressPercent;

    if (Beneficiary.IsValid())
    {
        auto* PlayerController = Cast<AQLPlayerController>(Beneficiary->GetController());
        if (PlayerController)
        {
            auto* UMG = PlayerController->GetUMG();
            if (UMG)
            {
                UMG->UpdateQuadDamageProgress(DisplayedProgressPercent);
            }
        }
    }
}