//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------


#include "QLWeaponManager.h"
#include "QLWeapon.h"
#include "QLCharacter.h"
#include "QLUtility.h"
#include "QLPlayerController.h"
#include "QLHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLWeaponManager::UQLWeaponManager() :
User(nullptr),
DamageMultiplier(1.0f),
bIsGlowing(false)
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLWeaponManager::SetUser(AQLCharacter* Character)
{
    User = Character;
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLCharacter* UQLWeaponManager::GetUser()
{
    return User.Get();
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLWeaponManager::SetCurrentWeapon(const FName& QLName)
{
    // find if the named weapon is in the inventory
    AQLWeapon* WeaponWanted = nullptr;

    for (const auto& Item : WeaponList)
    {
        if (QLName == Item->GetQLName())
        {
            WeaponWanted = Item;
            break;
        }
    }

    // if it is not, do nothing
    if (!WeaponWanted)
    {
        return;
    }

    // if the current weapon exists, hide it
    if (CurrentWeapon.IsValid())
    {
        CurrentWeapon->PrepareForImpendingWeaponSwitch();
        CurrentWeapon->GetGunSkeletalMeshComponent()->SetVisibility(false);
    }

    // change current weapon
    CurrentWeapon = WeaponWanted;
    CurrentWeapon->GetGunSkeletalMeshComponent()->SetVisibility(true);

    // change cross-hair
    UpdateCrossHair();
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLWeaponManager::UpdateCrossHair()
{
    if (!User.IsValid())
    {
        return;
    }

    AController* MyController = User->GetController();
    if (MyController)
    {
        AQLPlayerController* MyQLPlayerController = Cast<AQLPlayerController>(MyController);
        if (MyQLPlayerController)
        {
            AHUD* MyHUD = MyQLPlayerController->GetHUD();
            if (MyHUD)
            {
                AQLHUD* MyQLHUD = Cast<AQLHUD>(MyHUD);
                if (MyQLHUD)
                {
                    MyQLHUD->UpdateCrossHair(CurrentWeapon->GetCrossHairTexture());
                }
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeapon* UQLWeaponManager::GetCurrentWeapon()
{
    return CurrentWeapon.Get();
}

//------------------------------------------------------------
// todo: RemoveWeapon
//------------------------------------------------------------
void UQLWeaponManager::AddWeapon(AQLWeapon* Weapon)
{
    if (!User.IsValid())
    {
        return;
    }

    // if the weapon is already in the list, do not add
    for (auto& Item : WeaponList)
    {
        if (Item->GetQLName() == Weapon->GetQLName())
        {
            QLUtility::Log("UQLWeaponManager:: Weapon of the same type has already been added.");
            return;
        }
    }

    WeaponList.Add(Weapon);
    Weapon->SetWeaponManager(this);

    // set up the weapon
    auto* RootSphereComponent = Weapon->GetRootSphereComponent();
    if (RootSphereComponent)
    {
        RootSphereComponent->SetCollisionProfileName(TEXT("NoCollision"));
    }

    auto* gunMesh = Weapon->GetGunSkeletalMeshComponent();
    if (gunMesh)
    {
        gunMesh->CastShadow = false;
        gunMesh->bCastDynamicShadow = false;
    }

    Weapon->GetGunSkeletalMeshComponent()->SetVisibility(false);
    Weapon->SetActorEnableCollision(false);
    Weapon->DisableComponentsSimulatePhysics();
    Weapon->SetConstantRotationEnabled(false);
    Weapon->SetDamageMultiplier(DamageMultiplier);
    Weapon->AttachToComponent(User->GetFirstPersonMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

    if (bIsGlowing)
    {
        Weapon->StartGlow(GlowColor);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLWeaponManager::SetDamageMultiplier(const float Value)
{
    DamageMultiplier = Value;

    for (auto& Item : WeaponList)
    {
        Item->SetDamageMultiplier(DamageMultiplier);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLWeaponManager::StartGlowWeapon(const FLinearColor& Color)
{
    GlowColor = Color;

    bIsGlowing = true;

    for (auto& Item : WeaponList)
    {
        Item->StartGlow(GlowColor);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLWeaponManager::StopGlowWeapon()
{
    bIsGlowing = false;

    for (auto& Item : WeaponList)
    {
        Item->StopGlow();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLWeaponManager::CreateAndAddAllWeapons(const TArray<TSubclassOf<AQLWeapon>>& WeaponClassList)
{
    if (!User.IsValid())
    {
        return;
    }

    for (const auto& Item : WeaponClassList)
    {
        auto* Weapon = GetWorld()->SpawnActor<AQLWeapon>(Item, FVector::ZeroVector, FRotator::ZeroRotator);
        AddWeapon(Weapon);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLWeaponManager::SetCurrentWeaponVisibility(const bool bFlag)
{
    if (CurrentWeapon.IsValid())
    {
        CurrentWeapon->GetGunSkeletalMeshComponent()->SetVisibility(bFlag);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
bool UQLWeaponManager::HasWeapon(const FName& WeaponName)
{
    for (auto& Item : WeaponList)
    {
        if (Item->GetQLName() == WeaponName)
        {
            return true;
        }
    }

    return false;
}

