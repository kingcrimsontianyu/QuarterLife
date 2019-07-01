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
#include "QLRocketProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class AQLPlayerController;

//------------------------------------------------------------
// In Blueprint, set these properties
// - explosion particle system
// - explosion sound
//
// In addition, set this property of the landscape
// - generate overlap events = true
//------------------------------------------------------------
UCLASS()
class QL_API AQLRocketProjectile : public AQLProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLRocketProjectile();
};
