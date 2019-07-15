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
#include "QLPickup.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

//------------------------------------------------------------
// The AQLPickup actor is given a custom collision channel
// Project settings --> collision --> add object channels / trace channels
//------------------------------------------------------------
UCLASS()
class QL_API AQLPickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLPickup();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PlaySoundFireAndForget(const FName& SoundName);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PlaySound(const FName& SoundName);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void StopSound();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void PlayAnimationMontage(const FName& AnimationMontageName);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetConstantRotationEnabled(const bool bFlag);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    USphereComponent* GetRootSphereComponent();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FLinearColor GetGlowColor();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FName GetQLName();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    UStaticMeshComponent* GetStaticMeshComponent();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void ChangePhysicsSetup();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void RevertPhysicsSetup();

    //------------------------------------------------------------
    // After Delay seconds, perform PerformRotationInterpCallback()
    // which sets bStartRotationInterp to true, and performs rotation interpolation
    // in Tick() until the rotation becomes FRotator::ZeroRotator
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PerformRotationInterpWithDelay(const float Delay);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION()
    virtual void OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION()
    virtual void OnComponentHitImpl(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION()
    void PerformRotationInterpCallback();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    USphereComponent* RootSphereComponent;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UStaticMeshComponent* StaticMeshComponent;

    //------------------------------------------------------------
    // degree to rotate by per second
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    FRotator RotationRate;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UAudioComponent* SoundComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TMap<FName, USoundBase*> SoundList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    USoundAttenuation* SoundAttenuation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TMap<FName, UAnimMontage*> AnimationMontageList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    bool bCanBeRespawned;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    bool bConstantlyRotating;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float RespawnInterval;

    FTimerHandle RespawnTimerHandle;

    UPROPERTY()
    TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    FLinearColor GlowColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    FName QLName;

    FTimerHandle StartRotationDelayTimerHandle;

    bool bStartRotationInterp;
};
