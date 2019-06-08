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

class AQLCharacter;

//----------------------------------------
//----------------------------------------
UCLASS(Abstract)
class QL_API AQLWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLWeapon();

    // Called every frame
    virtual void Tick(float DeltaTime) override;
    virtual void Fire() PURE_VIRTUAL(AQLWeapon::Fire, );
    virtual void FireReleased() PURE_VIRTUAL(AQLWeapon::FireReleased, );
    virtual void FireRepeat() PURE_VIRTUAL(AQLWeapon::FireRepeat, );

    virtual void AltFire() PURE_VIRTUAL(AQLWeapon::AltFire, );
    virtual void AltFireReleased() PURE_VIRTUAL(AQLWeapon::AltFireReleased, );
    virtual void AltFireRepeat() PURE_VIRTUAL(AQLWeapon::AltFireRepeat, );

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetUser(AQLCharacter* Character);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void UnsetUser();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PlayFireSound(const FName& FireSoundName);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PlayFireAnimation(const FName& FireAnimationName);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TMap<FName, USoundBase*> FireSoundList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TMap<FName, UAnimMontage*> FireAnimationList;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    class USphereComponent* RootSphereComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    USkeletalMeshComponent* QLSkeletalMeshComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void PostInitializeComponents() override;

    FName Name;
    AQLCharacter* User;
};
