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
#include "GameFramework/Character.h"
#include "QLCharacter.generated.h"

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

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

protected:
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
    void OnFire();
public:
    // Returns Mesh1P subobject
    FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
    // Returns FirstPersonCameraComponent subobject
    FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

    // Pawn mesh: 1st person view (arms; seen only by self)
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
    class USkeletalMeshComponent* Mesh1P;

    // Gun mesh: 1st person view (seen only by self)
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
    class USkeletalMeshComponent* FP_Gun;

    // Location on gun mesh where projectiles should spawn.
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
    class USceneComponent* FP_MuzzleLocation;

    // First person camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FirstPersonCameraComponent;

    // Base turn rate, in deg/sec. Other scaling may affect final turn rate.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseTurnRate;

    // Base look up/down rate, in deg/sec. Other scaling may affect final rate.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float BaseLookUpRate;

    // Gun muzzle's offset from the characters location
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    FVector GunOffset;

    // Projectile class to spawn
    // UPROPERTY(EditDefaultsOnly, Category = Projectile)
    // TSubclassOf<class ATestFirstPersonProjectile> ProjectileClass;

    // Sound to play each time we fire
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    class USoundBase* FireSound;

    // AnimMontage to play each time we fire
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    class UAnimMontage* FireAnimation;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetHealth() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetMaxHealth() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetArmor() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetMaxArmor() const;
protected:
    float Health;
    float MaxHealth;
    float Armor;
    float MaxArmor;
};
