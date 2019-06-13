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
#include "QLRocketProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
//------------------------------------------------------------
// In Blueprint, set these properties
// - explosion particle system
// - explosion sound
//
// In addition, set this property of the landscape
// - generate overlap events = true
//------------------------------------------------------------
UCLASS()
class QL_API AQLRocketProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLRocketProjectile();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    UProjectileMovementComponent* GetProjectileMovementComponent();
protected:
    UFUNCTION()
    void OnBeginOverlapForComponent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    USphereComponent* RootSphereComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float RocketLifeSpan;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float RocketSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    UParticleSystem* ExplosionParticleSystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    USoundBase* ExplosionSound;
};