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
class QL_API AQLRecyclerGrenadeProjectile : public AQLProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLRecyclerGrenadeProjectile();

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
