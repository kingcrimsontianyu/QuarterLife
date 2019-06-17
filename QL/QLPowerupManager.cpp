//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------



#include "QLPowerupManager.h"
#include "QLPowerup.h"
#include "QLCharacter.h"
#include "QLUtility.h"

//------------------------------------------------------------
//------------------------------------------------------------
UQLPowerupManager::UQLPowerupManager() :
User(nullptr)
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLPowerupManager::SetUser(AQLCharacter* Character)
{
    User = Character;
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLCharacter* UQLPowerupManager::GetUser()
{
    return User.Get();
}

//------------------------------------------------------------
//------------------------------------------------------------
bool UQLPowerupManager::AddPowerup(AQLPowerup* Powerup)
{
    if (!Powerup)
    {
        return false;
    }

    if (!User.IsValid())
    {
        return false;
    }

    // if the powerup is already in the list, do not add
    for (auto& Item : PowerupList)
    {
        if (Item->GetPowerupName() == Powerup->GetPowerupName())
        {
            QLUtility::Log("UQLPowerupManager:: Powerup of the same type has already been added.");
            return false;
        }
    }

    PowerupList.Add(Powerup);
    Powerup->SetPowerupManager(this);
    return true;
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLPowerupManager::RemovePowerup(AQLPowerup* Powerup)
{
    if (!Powerup)
    {
        return;
    }

    if (!User.IsValid())
    {
        return;
    }

    int32 Index = -1;
    for (Index = 0; Index < PowerupList.Num(); ++Index)
    {
        if (PowerupList[Index]->GetPowerupName() == Powerup->GetPowerupName())
        {
            break;
        }
    }

    if (Index >= 0)
    {
        PowerupList.RemoveAt(Index);
    }

    Powerup->SetPowerupManager(nullptr);
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLPowerup* UQLPowerupManager::GetTopPowerup()
{
    if (PowerupList.Num() > 0)
    {
        return PowerupList.Last();
    }
    else
    {
        return nullptr;
    }
}