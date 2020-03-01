//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLAbilityTimeTravel.h"
#include "QLUtility.h"
#include "QLAbilityManager.h"
#include "QLCharacter.h"
#include "Components/PostProcessComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLAbilityTimeTravel::AQLAbilityTimeTravel()
{
    QLName = FName(TEXT("TimeTravel"));
    PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
    PostProcessComponent->bEnabled = false;

    TimeTravelTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimeTravelTimeline"));
    TimeTravelTimelineInterpFunction.BindUFunction(this, FName(TEXT("TimeTravelCallback")));

    SoundIdx = 0;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityTimeTravel::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityTimeTravel::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (PostProcessComponent)
    {
        if (MaterialTimeTravel)
        {
            FWeightedBlendable WeightedBlendable;
            DynamicMaterialTimeTravel = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, MaterialTimeTravel);
            WeightedBlendable.Object = DynamicMaterialTimeTravel.Get();
            WeightedBlendable.Weight = 1.0f;
            PostProcessComponent->Settings.WeightedBlendables.Array.Add(WeightedBlendable);
        }
    }

    if (TimeTravelTimeline && TimeTravelCurve)
    {
        TimeTravelTimeline->AddInterpFloat(TimeTravelCurve, TimeTravelTimelineInterpFunction, FName(TEXT("TimeTravel")));
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityTimeTravel::OnUse()
{
    Super::OnUse();

    if (!NearActor.IsValid() ||
        !FarActor.IsValid() ||
        !AbilityManager.IsValid() ||
        !AbilityManager->GetUser())
    {
        return;
    }

    AQLCharacter* MyCharacter = AbilityManager->GetUser();

    // check which actor, near or far is actually close to the actor
    // and use that actor as the near actor
    // use distance squared directly to reduce calculation
    float d1 = FVector::DistSquared(MyCharacter->GetActorLocation(), NearActor->GetActorLocation());
    float d2 = FVector::DistSquared(MyCharacter->GetActorLocation(), FarActor->GetActorLocation());
    if (d1 > d2)
    {
        SwapNearAndFarActor();
    }


    FVector NewLocation = MyCharacter->GetActorLocation() - NearActor->GetActorLocation() + FarActor->GetActorLocation();
    bool bTeleportSuccess = MyCharacter->TeleportTo(NewLocation,
        FRotator::ZeroRotator,
        false, // not a test, but actual teleport
        false); // check if the actor can be teleported

    if (!bTeleportSuccess)
    {
        return;
    }

    // now that teleport succeeds
    // do not rotate character
    // instead rotate the controller
    FRotator NewRotation = MyCharacter->GetController()->GetControlRotation() - NearActor->GetActorRotation() + FarActor->GetActorRotation();
    MyCharacter->GetController()->SetControlRotation(NewRotation);

    SwapNearAndFarActor();

    // enable postprocessing effect
    PostProcessComponent->bEnabled = true;

    // play sound
    constexpr char* MySoundNames[2] = { "OutsiderAppear", "OutsiderDisappear" };
    PlaySoundFireAndForget(FName(MySoundNames[SoundIdx]));

    // change sound index for the next play
    if (SoundIdx == 0)
    {
        SoundIdx = 1;
    }
    else if (SoundIdx == 1)
    {
        SoundIdx = 0;
    }

    // postprocessing animation
    if (TimeTravelTimeline && TimeTravelCurve)
    {
        TimeTravelTimeline->PlayFromStart();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityTimeTravel::OnAbilityEnd()
{
    Super::OnAbilityEnd();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityTimeTravel::SetNearAndFarActors(AActor* NearActorExt, AActor* FarActorExt)
{
    NearActor = NearActorExt;
    FarActor = FarActorExt;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityTimeTravel::SwapNearAndFarActor()
{
    TWeakObjectPtr<AActor> Temp = NearActor;
    NearActor = FarActor;
    FarActor = Temp;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityTimeTravel::TimeTravelCallback(float Val)
{
    if (DynamicMaterialTimeTravel.IsValid())
    {
        DynamicMaterialTimeTravel->SetScalarParameterValue("InterpParam", Val);
    }
}