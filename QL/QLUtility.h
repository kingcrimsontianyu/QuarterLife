//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------

#pragma once

#include <sstream>
#include <string>
#include "CoreMinimal.h"

namespace QLUtility
{
    //------------------------------------------------------------
    //------------------------------------------------------------
    void Log(const FString& msg);

    //------------------------------------------------------------
    //------------------------------------------------------------
    void Log(const float Value);

    //------------------------------------------------------------
    //------------------------------------------------------------
    void Log(const FVector& Value);

    //------------------------------------------------------------
    //------------------------------------------------------------
    void Log(const FRotator& Value);

    //------------------------------------------------------------
    //------------------------------------------------------------
    void Log(const bool bValue);

    //------------------------------------------------------------
    //------------------------------------------------------------
    void Screen(const FString& msg);

    //------------------------------------------------------------
    // Given a pointer, convert the address to FString
    //------------------------------------------------------------
    template <typename T>
    FString GetAddressInFString(T* addr)
    {
        const void* address = static_cast<const void*>(addr);
        std::stringstream ss;
        ss << address;
        std::string name = ss.str();
        return FString(name.c_str());
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void MakePredictionShot(
        FVector& LocationProjectileHitsEnemy,
        float& TimeProjectileHitsEnemy,
        const FVector MyLocation,
        const FVector EnemyLocation,
        const FVector EnemyVelocity,
        const float ProjectileSpeed);

    //------------------------------------------------------------
    //------------------------------------------------------------
    FVector SamplePointFromDiskOnXYPlane(const float Radius, const FVector& Center);

    //------------------------------------------------------------
    //------------------------------------------------------------
    FVector SamplePointFromSquareOnXYPlane(const float XHalfSide, const float YHalfSide, const FVector& Center);

    //------------------------------------------------------------
    //------------------------------------------------------------
    template<typename TEnum>
    static FString GetEnumValueAsString(const FString& Name, TEnum Value)
    {
        const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
        if (!enumPtr)
        {
            return FString("Invalid");
        }
        return enumPtr->GetNameByValue((int64)Value).ToString();
    }

}
