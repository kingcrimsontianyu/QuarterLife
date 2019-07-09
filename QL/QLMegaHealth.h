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

#include "CoreMinimal.h"
#include "QLHealth.h"
#include "QLMegaHealth.generated.h"

//------------------------------------------------------------
// In Blueprint,
// Set collision object type to QLPickup
//------------------------------------------------------------
UCLASS()
class QL_API AQLMegaHealth : public AQLHealth
{
	GENERATED_BODY()

public:
    AQLMegaHealth();

protected:
};
