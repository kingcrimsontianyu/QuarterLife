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
class ADecalActor;

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

    virtual void OnBeginOverlapForComponent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    UPROPERTY(EditDefaultsOnly, Category = "C++Property")
    TSubclassOf<ADecalActor> DecalClass;
};
