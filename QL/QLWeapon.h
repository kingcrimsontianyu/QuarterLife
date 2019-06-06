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
#include "QLWeapon.generated.h"

UCLASS(Abstract)
class QL_API AQLWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    virtual void Fire() PURE_VIRTUAL(AQLWeapon::Fire, );
    virtual void FireReleased() PURE_VIRTUAL(AQLWeapon::FireReleased, );
    virtual void FireRepeat() PURE_VIRTUAL(AQLWeapon::FireRepeat, );

    virtual void AltFire() PURE_VIRTUAL(AQLWeapon::AltFire, );
    virtual void AltFireReleased() PURE_VIRTUAL(AQLWeapon::AltFireReleased, );
    virtual void AltFireRepeat() PURE_VIRTUAL(AQLWeapon::AltFireRepeat, );

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    class USphereComponent* RootSphereComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    USkeletalMeshComponent* QLSkeletalMeshComponent;
protected:
    FName Name;
};
