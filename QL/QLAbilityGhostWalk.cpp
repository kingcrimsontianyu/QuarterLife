//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLAbilityGhostWalk.h"
#include "Components/PostProcessComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Classes/Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "QLUtility.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "QLAbilityManager.h"
#include "QLCharacter.h"
#include "QLPlayerController.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLAbilityGhostWalk::AQLAbilityGhostWalk()
{
    QLName = FName(TEXT("GhostWalk"));
    PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
    PostProcessComponent->bEnabled = false;
    TelefragDamage = 8000.0f;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityGhostWalk::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityGhostWalk::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityGhostWalk::OnUse()
{
    Super::OnUse();

    if (!IsActive())
    {
        return;
    }

    PostProcessComponent->bEnabled = true;
    PlaySoundFireAndForget(FName(TEXT("Voiceline")));
    PlaySoundFireAndForget(FName(TEXT("Swoosh")));

    Deactivate();

    // in the duration of ghost walk, the player can overlap (instead of collide) with enemies
    // however, all overlap events are disabled to prevent players from picking up items
    if (AbilityManager.IsValid())
    {
        auto* QLCharacter = AbilityManager->GetUser();
        if (QLCharacter)
        {
            auto* CapsuleComponent = QLCharacter->GetCapsuleComponent();
            if (CapsuleComponent)
            {
                CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
                CapsuleComponent->SetGenerateOverlapEvents(false);
            }

            QLCharacter->SetFireEnabled(false);
            QLCharacter->GetFirstPersonMesh()->SetVisibility(false);
            QLCharacter->GetThirdPersonMesh()->SetVisibility(false);
            QLCharacter->SetCurrentWeaponVisibility(false);
            QLCharacter->SetSwitchWeaponEnabled(false);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLAbilityGhostWalk::OnAbilityEnd()
{
    Super::OnAbilityEnd();

    PlaySoundFireAndForget(FName(TEXT("Swoosh")));

    PostProcessComponent->bEnabled = false;

    if (AbilityManager.IsValid())
    {
        auto* QLCharacter = AbilityManager->GetUser();
        if (QLCharacter)
        {
            // reset collision part 1
            auto* CapsuleComponent = QLCharacter->GetCapsuleComponent();
            if (CapsuleComponent)
            {
                // todo: for some reason (UE 4.22.2), SetGenerateOverlapEvents(true) alone does not work !!!
                CapsuleComponent->SetGenerateOverlapEvents(true);
                CapsuleComponent->SetCollisionProfileName(FName(TEXT("Pawn")));
                CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
            }

            // telefrag overlapping characters
            TArray<AActor*> Results;
            QLCharacter->GetOverlappingActors(Results, QLCharacter->GetClass());
            for (const auto& Item : Results)
            {
                // create a damage event
                const FDamageEvent DamageEvent;

                float DamageAmount = Item->TakeDamage(TelefragDamage, DamageEvent, QLCharacter->GetController(), this);

                // display damage
                AQLPlayerController* QLPlayerController = QLCharacter->GetQLPlayerController();
                if (DamageAmount > 0.0f && QLPlayerController)
                {
                    UCameraComponent* CameraComponent = QLCharacter->GetFirstPersonCameraComponent();
                    if (CameraComponent)
                    {
                        FVector Location = CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * 100.0f;
                        QLPlayerController->ShowDamageOnScreen(DamageAmount, Location);
                    }
                }
            }

            // reset collision part 2
            if (CapsuleComponent)
            {
                CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
            }

            QLCharacter->SetFireEnabled(true);
            QLCharacter->GetFirstPersonMesh()->SetVisibility(true);
            QLCharacter->GetThirdPersonMesh()->SetVisibility(true);
            QLCharacter->SetCurrentWeaponVisibility(true);
            QLCharacter->SetSwitchWeaponEnabled(true);
        }
    }
}
