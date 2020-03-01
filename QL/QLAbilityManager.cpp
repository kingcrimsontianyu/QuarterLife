//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------

#include "QLAbilityManager.h"
#include "QLAbility.h"
#include "QLCharacter.h"
#include "QLUtility.h"
#include "QLPlayerController.h"
#include "QLHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "QLAbilityTimeTravel.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLAbilityManager::UQLAbilityManager() :
User(nullptr),
DamageMultiplier(1.0f)
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLAbilityManager::SetUser(AQLCharacter* Character)
{
    User = Character;
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLCharacter* UQLAbilityManager::GetUser()
{
    return User.Get();
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLAbilityManager::SetCurrentAbility(const FName& QLName)
{
    // find if the named Ability is in the inventory
    AQLAbility* AbilityWanted = nullptr;

    for (const auto& Item : AbilityList)
    {
        if (QLName == Item->GetQLName())
        {
            AbilityWanted = Item;
            break;
        }
    }

    // if it is not, do nothing
    if (!AbilityWanted)
    {
        QLUtility::Log("Named ability not found : " + QLName.ToString());
        return;
    }

    // if the current Ability exists, hide it
    if (CurrentAbility.IsValid())
    {
        //CurrentAbility->GetGunSkeletalMeshComponent()->SetVisibility(false);
    }

    // change current Ability
    CurrentAbility = AbilityWanted;
    //CurrentAbility->GetGunSkeletalMeshComponent()->SetVisibility(true);
}


//------------------------------------------------------------
//------------------------------------------------------------
AQLAbility* UQLAbilityManager::GetCurrentAbility()
{
    return CurrentAbility.Get();
}

//------------------------------------------------------------
// todo: RemoveAbility
//------------------------------------------------------------
void UQLAbilityManager::AddAbility(AQLAbility* Ability)
{
    if (!User.IsValid())
    {
        return;
    }

    // if the Ability is already in the list, do not add
    for (auto& Item : AbilityList)
    {
        if (Item->GetQLName() == Ability->GetQLName())
        {
            QLUtility::Log("UQLAbilityManager:: Ability of the same type has already been added.");
            return;
        }
    }

    AbilityList.Add(Ability);
    Ability->SetAbilityManager(this);

    // set up the Ability
    auto* RootSphereComponent = Ability->GetRootSphereComponent();
    if (RootSphereComponent)
    {
        RootSphereComponent->SetCollisionProfileName(TEXT("NoCollision"));
    }

    auto* abilityMesh = Ability->GetStaticMeshComponent();
    if (abilityMesh)
    {
        abilityMesh->SetVisibility(false);
    }

    Ability->SetActorEnableCollision(false);
    Ability->DisableComponentsSimulatePhysics();
    Ability->SetConstantRotationEnabled(false);
    Ability->SetDamageMultiplier(DamageMultiplier);
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLAbilityManager::SetDamageMultiplier(const float Value)
{
    DamageMultiplier = Value;

    for (auto& Item : AbilityList)
    {
        Item->SetDamageMultiplier(DamageMultiplier);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLAbilityManager::CreateAndAddAllAbilities(const TArray<TSubclassOf<AQLAbility>>& AbilityClassList)
{
    if (!User.IsValid())
    {
        return;
    }

    for (const auto& Item : AbilityClassList)
    {
        auto* Ability = GetWorld()->SpawnActor<AQLAbility>(Item, FVector::ZeroVector, FRotator::ZeroRotator);
        AddAbility(Ability);
        User->SetCurrentAbility(Ability->GetQLName());
        Ability->UpdateProgressOnUMGInternal(1.0f);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLAbilityManager::SetNearAndFarActors(AActor* NearActorExt, AActor* FarActorExt)
{
    if (!User.IsValid())
    {
        return;
    }

    // find time travel ability
    AQLAbilityTimeTravel* Result = nullptr;
    for (const auto& Item : AbilityList)
    {
        Result = Cast<AQLAbilityTimeTravel>(Item);
        if (Result)
        {
            break;
        }
    }

    if (Result)
    {
        Result->SetNearAndFarActors(NearActorExt, FarActorExt);
    }
}

