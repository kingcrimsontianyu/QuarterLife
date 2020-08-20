//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLAbilityTheWorld.h"
#include "Components/PostProcessComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "QLUtility.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "QLAbilityManager.h"
#include "QLCharacter.h"
#include "QLPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLAbilityTheWorld::AQLAbilityTheWorld()
{
    QLName = FName(TEXT("TheWorld"));
    PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
    PostProcessComponent->bEnabled = false;

    // animation
    TheWorldTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TheWorldTimeline"));
    TheWorldTimelineInterpFunction.BindUFunction(this, FName(TEXT("TheWorldCallback")));

    TimeDilationFactor = 0.001f;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityTheWorld::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityTheWorld::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (PostProcessComponent)
    {
        if (MaterialTheWorld)
        {
            FWeightedBlendable WeightedBlendable;
            DynamicMaterialTheWorld = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, MaterialTheWorld);
            WeightedBlendable.Object = DynamicMaterialTheWorld.Get();
            WeightedBlendable.Weight = 1.0f;
            PostProcessComponent->Settings.WeightedBlendables.Array.Add(WeightedBlendable);
        }
    }

    if (TheWorldTimeline && TheWorldCurve)
    {
        TheWorldTimeline->AddInterpFloat(TheWorldCurve, TheWorldTimelineInterpFunction, FName(TEXT("TheWorld")));
    }

    // Timer always follows global time dilation, so to freeze time k seconds,
    // the abilility duraction should be set to (k * global time dilation)
    AbilityDuration = AbilityDuration * TimeDilationFactor;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityTheWorld::OnUse()
{
    Super::OnUse();

    if (!IsActive())
    {
        return;
    }

    PostProcessComponent->bEnabled = true;
    PlaySoundFireAndForget(FName(TEXT("ZaWarudo")));
    PlaySoundFireAndForget(FName(TEXT("VoicelineTheWorld")));


    // change global time dilation
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilationFactor);

    // to avoid the large spike of DeltaTime, here we introduce 0.1 second delay before displaying the postprocessing effect
    constexpr float ArtificialDelay = 0.1f;
    GetWorldTimerManager().SetTimer(DelayTimerHandle,
        this,
        &AQLAbilityTheWorld::OnTimeStopped,
        1.0f, // time interval in second
        false, // loop
        ArtificialDelay * TimeDilationFactor); // delay in second

    Deactivate();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityTheWorld::OnAbilityEnd()
{
    Super::OnAbilityEnd();

    PlaySoundFireAndForget(FName(TEXT("Expire")));

    PostProcessComponent->bEnabled = false;

    // reset time dilation
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
    AQLCharacter* QLCharacter = AbilityManager->GetUser();
    if (QLCharacter)
    {
        QLCharacter->CustomTimeDilation = 1.0f;

        //AController* Controller = QLCharacter->GetController();
        //if (Controller)
        //{
        //    Controller->CustomTimeDilation = 1.0f;
        //}

        CustomTimeDilation = 1.0f;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityTheWorld::TheWorldCallback(float Val)
{
    if (DynamicMaterialTheWorld.IsValid())
    {
        DynamicMaterialTheWorld->SetScalarParameterValue("RadiusScale", Val);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityTheWorld::OnTimeStopped()
{
    // change time dilation of those actors that are except from time stopping
    AQLCharacter* QLCharacter = AbilityManager->GetUser();
    if (QLCharacter)
    {
        float InverseTimeDilationFactor = 1.0f / TimeDilationFactor;

        QLCharacter->CustomTimeDilation = InverseTimeDilationFactor;

        //AController* Controller = QLCharacter->GetController();
        //if (Controller)
        //{
        //    Controller->CustomTimeDilation = InverseTimeDilationFactor;
        //}

        CustomTimeDilation = InverseTimeDilationFactor;
    }

    if (TheWorldTimeline && TheWorldCurve)
    {
        TheWorldTimeline->PlayFromStart();
    }
}
