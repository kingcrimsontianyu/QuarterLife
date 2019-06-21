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
#include "QLPickup.h"
#include "QLAbility.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLAbility : public AQLPickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLAbility();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetAbilityManager(UQLAbilityManager* AbilityManagerExt);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void SetDamageMultiplier(const float Value);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void OnUse();

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void UpdateProgressOnUMGInternal(const float Value);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void UpdateProgressOnUMG();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void Reactivate();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void Deactivate();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    bool IsActive();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void PostInitializeComponents() override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY()
    float DamageMultiplier;

    UPROPERTY()
    TWeakObjectPtr<UQLAbilityManager> AbilityManager;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    bool bCanBeUsed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float CooldownDuration;

    float CooldownTimeElapsed;

    FTimerHandle CooldownDurationTimerHandle;

    FTimerHandle CooldownTimeElapsedTimerHandle;

    float CooldownUpdateTimeIncrement;

    float CooldownPercent;
};
