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
#include "Components/TimelineComponent.h"
#include "QLRecyclerGrenadeProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class AQLPlayerController;
class UPostProcessComponent;

//------------------------------------------------------------
// The recycler grenade projectile has several stages
// idle
// attract
// annihilate
// recover
// destroy
//------------------------------------------------------------
UCLASS()
class QL_API AQLRecyclerGrenadeProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLRecyclerGrenadeProjectile();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    UProjectileMovementComponent* GetProjectileMovementComponent();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetQLPlayerController(AQLPlayerController* PlayerControllerExt);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetDamageMultiplier(const float Value);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void PostInitializeComponents() override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void OnProjectileBounceImpl(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void Implode();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void Attract();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void Annihilate();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void CalculateMaterialParameter();

    UFUNCTION()
    void SpaceWarpCallback(float Value);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    USphereComponent* RootSphereComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY()
    TWeakObjectPtr<AQLPlayerController> PlayerController;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    USoundBase* BounceSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float GrenadeSpeed;

    UPROPERTY()
    float DamageMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float BasicDamage;

    UPROPERTY()
    float BasicDamageAdjusted;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++Property")
    UPostProcessComponent* PostProcessComponent;

    UPROPERTY()
    TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterialSpaceWarp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    UMaterialInterface* MaterialSpaceWarp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float IdleDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float AttractDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float AttractInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float RecoverDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float BlastRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float BlastSpeedChange;

    FTimerHandle IdleTimerHandle;
    FTimerHandle ImplodeTimerHandle;
    FTimerHandle AttractTimerHandle;

    UPROPERTY()
    UTimelineComponent* SpaceWarpTimeline;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++Property")
    UCurveFloat* SpaceWarpCurve;

    FOnTimelineFloat SpaceWarpTimelineInterpFunction;

    bool bCalculateMaterialParameter;
};
