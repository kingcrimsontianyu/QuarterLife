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
#include "GameFramework/Actor.h"
#include "QLWeapon.generated.h"

class AQLCharacter;
class UAudioComponent;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLWeapon();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void OnFire();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void OnFireRelease();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void OnFireHold();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void OnAltFire();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void OnAltFireRelease();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void OnAltFireHold();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PlayFireSound(const FName& FireSoundName);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void StopFireSound();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PlayFireAnimation(const FName& FireAnimationName);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    USkeletalMeshComponent* GetGunSkeletalMeshComponent();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FVector GetMuzzleLocation();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FName GetWeaponName();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void InflictDamage();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetHitRange(float HitRange);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    UTexture2D* GetCrossHairTexture(const FName& CrossHairTextureName = "Default");

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetWeaponManager(UQLWeaponManager* WeaponManagerExt);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    UQLWeaponManager* GetWeaponManager();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void PostInitializeComponents() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UAudioComponent* FireSoundComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TMap<FName, UTexture2D*> CrosshairTextureList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TMap<FName, USoundBase*> FireSoundList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TMap<FName, UAnimMontage*> FireAnimationList;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    class USphereComponent* RootSphereComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    USkeletalMeshComponent* GunSkeletalMeshComponent;

    // Location on gun mesh where projectiles should spawn.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    USceneComponent* MuzzleSceneComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UParticleSystemComponent* BeamComponent;

    FName WeaponName;

    FTimerHandle HeldDownFireTimerHandle;

    float HitRange;

    float RateOfFire;

    bool bIsFireHeld;

    UQLWeaponManager* WeaponManager;
};
