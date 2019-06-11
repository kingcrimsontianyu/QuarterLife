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
void UQLWeaponManager::SetCurrentWeapon(const FName& WeaponName)
{
    AQLWeapon** Result = WeaponList.Find(WeaponName);
    if (Result == nullptr)
    {
        CurrentWeapon = nullptr;
        QLUtility::Log("UQLWeaponManager::SetCurrentWeapon failed");
        return;
    }

    if (*Result == nullptr)
    {
        CurrentWeapon = nullptr;
        QLUtility::Log("UQLWeaponManager::SetCurrentWeapon failed");
        return;
    }

    CurrentWeapon = *Result;

    // change mesh
    auto* gunMesh = CurrentWeapon->GetGunSkeletalMeshComponent();
    gunMesh->CastShadow = false;
    gunMesh->bCastDynamicShadow = false;

    // attach actor to component
    CurrentWeapon->AttachToComponent(User->GetFirstPersonMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

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
//------------------------------------------------------------
void UQLWeaponManager::AddWeapon(AQLWeapon* Weapon)
{
    WeaponList.Add(Weapon->GetWeaponName(), Weapon);
}


