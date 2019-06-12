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
    AQLWeapon** Result = WeaponList.Find(WeaponName);
    if (Result == nullptr)
    {
        CurrentWeapon = nullptr;
        return;
    }

    if (*Result == nullptr)
    {
        CurrentWeapon = nullptr;
        return;
    }

    CurrentWeapon = *Result;

    // set up the weapon
    CurrentWeapon->SetActorHiddenInGame(false);
    for (auto&& Elem : WeaponList)
    {
        if (Elem.Value != CurrentWeapon)
        {
            Elem.Value->SetActorHiddenInGame(true);
        }
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
    WeaponList.Add(Weapon->GetWeaponName(), Weapon);
    Weapon->SetWeaponManager(this);

    // set up the weapon
    auto* gunMesh = Weapon->GetGunSkeletalMeshComponent();
    gunMesh->CastShadow = false;
    gunMesh->bCastDynamicShadow = false;

    Weapon->AttachToComponent(User->GetFirstPersonMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

    Weapon->SetActorHiddenInGame(true);
    Weapon->SetActorEnableCollision(false);
}


