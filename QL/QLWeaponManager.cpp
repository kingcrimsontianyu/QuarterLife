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
void UQLWeaponManager::SetCurrentWeapon(const EQLWeapon WeaponType)
{
    // find if the named weapon is in the inventory
    AQLWeapon* WeaponWanted = nullptr;

    for (const auto& Item : WeaponList)
    {
        if (WeaponType == Item->GetWeaponType())
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
        CurrentWeapon->StopFire();
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

    if (User->GetIsBot())
    {
        Weapon->AttachToComponent(User->GetThirdPersonMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GripPoint"));
        // hardcoded adjustment
        // todo: come up with a more elegant solution!
        Weapon->SetActorRelativeLocation(FVector(-2.0f, 8.0f, -4.0f));
        Weapon->SetActorRelativeRotation(FRotator(0.0f, -155.0f, 5.0f)); // argument order: pitch, yaw, roll
    }
    else
    {
        if (Weapon->GetQLName() == FName(TEXT("Gauntlet")))
        {
            Weapon->AttachToComponent(User->GetFirstPersonMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GripPointDrill"));
        }
        else
        {
            Weapon->AttachToComponent(User->GetFirstPersonMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GripPoint"));
        }
    }

    if (bIsGlowing)
    {
        Weapon->StartGlow(GlowColor);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLWeaponManager::DestroyAllWeapon()
{
    for (auto& Item : WeaponList)
    {
        Item->Destroy();
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
bool UQLWeaponManager::HasWeapon(const EQLWeapon WeaponType)
{
    for (auto& Item : WeaponList)
    {
        if (Item->GetWeaponType() == WeaponType)
        {
            return true;
        }
    }

    return false;
}

