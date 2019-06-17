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
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "QLUtility.h"
#include "QLCharacter.h"
#include "TimerManager.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLPowerupQuadDamage::AQLPowerupQuadDamage()
{
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    BoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
    BoxComponent->SetSimulatePhysics(false);
    BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);
    StaticMeshComponent->SetSimulatePhysics(false);
    StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AQLPowerupQuadDamage::OnComponentBeginOverlapImpl);
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

            constexpr float DamageMultiplier = 4.0f;
            Beneficiary->SetDamageMultiplier(DamageMultiplier);
            Beneficiary->StartGlow(GlowColor);

            Deactivate();

            // countdown of the next respawn
            GetWorldTimerManager().SetTimer(RespawnTimerHandle,
                this,
                &AQLPowerupQuadDamage::Reactivate,
                1.0f, // time interval in second
                false, // loop
                RespawnInterval); // delay in second

            // take effect immediately
            GetWorldTimerManager().SetTimer(EffectTimerHandle,
                this,
                &AQLPowerupQuadDamage::OnEffectGone,
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
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AQLPowerupQuadDamage::OnComponentBeginOverlapImpl);
    SetActorHiddenInGame(false);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupQuadDamage::Deactivate()
{
    // temporarily remove delegate until next time quad shows up
    BoxComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AQLPowerupQuadDamage::OnComponentBeginOverlapImpl);

    // hide quad as if it has been consumed by the player
    SetActorHiddenInGame(true);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPowerupQuadDamage::OnEffectGone()
{
    // clean up
    if (Beneficiary.IsValid())
    {
        Beneficiary->SetDamageMultiplier(1.0f);
        Beneficiary->StopGlow();
    }

    // reset the weak pointer
    Beneficiary.Reset();
}