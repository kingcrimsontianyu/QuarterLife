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

//------------------------------------------------------------
//------------------------------------------------------------
AQLPowerupProtection::AQLPowerupProtection()
{
    RootSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AQLPowerupProtection::OnComponentBeginOverlapImpl);

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
void AQLPowerupProtection::OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

            Beneficiary->SetProtectionMultiplier(ProtectionMultiplier);
            Beneficiary->StartGlow(GlowColor);

            Deactivate();

            // countdown of the next respawn
            GetWorldTimerManager().SetTimer(RespawnTimerHandle,
                this,
                &AQLPowerupProtection::Reactivate,
                1.0f, // time interval in second
                false, // loop
                RespawnInterval); // delay in second

            // take effect immediately
            GetWorldTimerManager().SetTimer(EffectTimerHandle,
                this,
                &AQLPowerupProtection::OnEffectGone,
                1.0f, // time interval in second
                false, // loop
                EffectDuration); // delay in second
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupProtection::Reactivate()
{
    RootSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AQLPowerupProtection::OnComponentBeginOverlapImpl);

    if (DynamicMaterial.IsValid())
    {
        DynamicMaterial->SetScalarParameterValue("GlowIntensity", 5.0f);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupProtection::Deactivate()
{
    // temporarily remove delegate until next time quad shows up
    RootSphereComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AQLPowerupProtection::OnComponentBeginOverlapImpl);

    if (DynamicMaterial.IsValid())
    {
        DynamicMaterial->SetScalarParameterValue("GlowIntensity", 0.1f);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupProtection::OnEffectGone()
{
    // clean up
    if (Beneficiary.IsValid())
    {
        Beneficiary->SetProtectionMultiplier(1.0f);
        Beneficiary->StopGlow();
    }

    // reset the weak pointer
    Beneficiary.Reset();
}