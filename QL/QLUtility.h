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
}
