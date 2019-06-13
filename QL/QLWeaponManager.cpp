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

//------------------------------------------------------------
//------------------------------------------------------------
UQLWeaponManager::UQLWeaponManager() :
User(nullptr),
CurrentWeapon(nullptr)
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
    return User;
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLWeaponManager::SetCurrentWeapon(const FName& WeaponName)
{
    // if current weapon exists, hide it
    if (CurrentWeapon)
    {
        CurrentWeapon->PrepareForImpendingWeaponSwitch();
        CurrentWeapon->GetGunSkeletalMeshComponent()->SetVisibility(false);
    }

    // find if the named weapon is in the inventory
    bool bFound = false;
    for (const auto& Item : WeaponList)
    {
        if (WeaponName == Item->GetWeaponName())
        {
            CurrentWeapon = Item;
            CurrentWeapon->GetGunSkeletalMeshComponent()->SetVisibility(true);
            bFound = true;
        }
    }

    if (!bFound)
    {
        CurrentWeapon = nullptr;
        FString msg = "UQLWeaponManager: Not found " + WeaponName.ToString();
        QLUtility::Log(msg);
        return;
    }

    // change cross-hair
    UpdateCrossHair();
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLWeaponManager::UpdateCrossHair()
{
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
    return CurrentWeapon;
}

//------------------------------------------------------------
// todo: RemoveWeapon
//------------------------------------------------------------
void UQLWeaponManager::AddWeapon(AQLWeapon* Weapon)
{
    WeaponList.Add(Weapon);
    Weapon->SetWeaponManager(this);

    // set up the weapon
    auto* gunMesh = Weapon->GetGunSkeletalMeshComponent();
    gunMesh->CastShadow = false;
    gunMesh->bCastDynamicShadow = false;

    Weapon->GetGunSkeletalMeshComponent()->SetVisibility(false);
    Weapon->SetActorEnableCollision(false);
    Weapon->DisableComponentsSimulatePhysics();

    Weapon->AttachToComponent(User->GetFirstPersonMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}


