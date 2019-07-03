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
#include "QLProjectile.h"
#include "QLNailProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class AQLPlayerController;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLNailProjectile : public AQLProjectile
{
	GENERATED_BODY()

public:
    AQLNailProjectile();

protected:
    virtual void PostInitializeComponents() override;
};
