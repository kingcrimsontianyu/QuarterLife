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
#include "GameFramework/Character.h"
#include "QLPlayerHealthArmorBarUserWidget.h"
#include "Components/TimelineComponent.h"
#include "QLCharacter.generated.h"

class AQLWeapon;
class UQLWeaponManager;
class AQLAbility;
class UQLAbilityManager;
class UWidgetComponent;
class UQLPowerupManager;

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AQLCharacter();

    // Called every frame
    // virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FHitResult RayTraceFromCharacterPOV(float rayTraceRange = 1e5f);

    // Returns FirstPersonMesh subobject
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    USkeletalMeshComponent* GetFirstPersonMesh();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    USkeletalMeshComponent* GetThirdPersonMesh();

    // Returns FirstPersonCameraComponent subobject
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    UCameraComponent* GetFirstPersonCameraComponent() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetHealthArmorBarVisible(bool bFlag);

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION()
    void TakeDamageQuakeStyle(float ActualDamage);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void UpdateHealth();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void UpdateArmor();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    AQLPlayerController* GetQLPlayerController();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void AddWeapon(AQLWeapon* Weapon);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetCurrentWeapon(const FName& QLName);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void AddAbility(AQLAbility* Ability);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetCurrentAbility(const FName& QLName);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    bool AddPowerup(AQLPowerup* Powerup);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void RemovePowerup(AQLPowerup* Powerup);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void Die();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void OnDie();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    bool IsAlive();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetHealth() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetMaxHealth() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetArmor() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetMaxArmor() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetDamageMultiplier(const float Value);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetProtectionMultiplier(const float Value);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void StartGlow();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void StopGlow();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetFireEnabled(const bool bFlag);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetSwitchWeaponEnabled(const bool bFlag);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetCurrentWeaponVisibility(const bool bFlag);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual UAnimSequence* PlayAnimationSequence(const FName& AnimationSequenceName);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float Armor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float MaxArmor;

protected:

    // Pawn mesh : 1st person view(arms; seen only by self)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    USkeletalMeshComponent* FirstPersonMesh;

    UPROPERTY()
    TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterialFirstPersonMesh;

    // Pawn mesh: 1st person view (arms; seen only by self)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    USkeletalMeshComponent* ThirdPersonMesh;

    UPROPERTY()
    TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterialThirdPersonMesh;

    // First person camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FirstPersonCameraComponent;

    // Base turn rate, in deg/sec. Other scaling may affect final turn rate.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float BaseTurnRate;

    // Base look up/down rate, in deg/sec. Other scaling may affect final rate.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float BaseLookUpRate;

    // Projectile class to spawn
    // UPROPERTY(EditDefaultsOnly, Category = Projectile)
    // TSubclassOf<class ATestFirstPersonProjectile> ProjectileClass;
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void PostInitializeComponents() override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Handles moving forward/backward
    void MoveForward(float Val);

    // Handles strafing movement, left and right
    void MoveRight(float Val);

    // Called via input to turn at a given rate.
    // This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
    void TurnAtRate(float Rate);

    // Called via input to turn look up/down at a given rate.
    // This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
    void LookUpAtRate(float Rate);

    // Fires a projectile.
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void OnFire();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void OnFireRelease();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void OnAltFire();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void OnAltFireRelease();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void OnRestartLevel();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SwitchToRocketLauncher();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SwitchToLightningGun();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SwitchToRailGun();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SwitchToPortalGun();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SwitchToGrenadeLauncher();

    // Fires a projectile.
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void OnUseAbility();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PlaySoundFireAndForget(const FName& SoundName);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PlaySound(const FName& SoundName);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void StopSound();
protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UWidgetComponent* PlayerHealthArmorBarWidgetComponent;

    UPROPERTY()
    UQLWeaponManager* WeaponManager;

    UPROPERTY()
    UQLAbilityManager* AbilityManager;

    UPROPERTY()
    UQLPowerupManager* PowerupManager;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float ProtectionMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TMap<FName, UAnimMontage*> AnimationMontageList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TMap<FName, UAnimSequence*> AnimationSequenceList;

    FTimerHandle DieTimerHandle;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UAudioComponent* SoundComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TMap<FName, USoundBase*> SoundList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TArray<TSubclassOf<AQLWeapon>> WeaponClassList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TArray<TSubclassOf<AQLAbility>> AbilityClassList;

    UPROPERTY()
    bool bCanFireAndAltFire;

    UPROPERTY()
    bool bCanSwitchWeapon;
};
