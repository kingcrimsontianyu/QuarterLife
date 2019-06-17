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
    virtual void PlayAnimation(const FName& AnimationName);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void EnableConstantRotation();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void DisableConstantRotation();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PerformConstantRotation();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    USphereComponent* GetRootSphereComponent();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

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
    TMap<FName, UAnimMontage*> AnimationList;

    FTimerHandle ConstantRotationTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    bool bCanBeRespawned;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float RespawnInterval;

    FTimerHandle RespawnTimerHandle;

    UPROPERTY()
    TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    FLinearColor GlowColor;
};
