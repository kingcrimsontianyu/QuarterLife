//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QLWeapon.h"
#include "QLPortalEnum.h"
#include "QLWeaponPortalGun.generated.h"

class AQLColoredPortal;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLWeaponPortalGun : public AQLWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLWeaponPortalGun();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

    virtual void Fire() override;

    virtual void AltFire() override;
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void CreatePortalIfConditionsAreMet(EPortalColor PortalColor);

    AQLColoredPortal* BluePortal;
    AQLColoredPortal* OrangePortal;

    UPROPERTY(EditDefaultsOnly, Category = "C++Property")
    TSubclassOf<AQLColoredPortal> PortalClass;

    float HitRange;
};
