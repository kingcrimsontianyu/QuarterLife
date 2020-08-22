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
#include "QLJumpPad.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class QL_API AQLJumpPad : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLJumpPad();

protected:
    //------------------------------------------------------------
	// Called when the game starts or when spawned
    //------------------------------------------------------------
	virtual void BeginPlay() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION()
    virtual void OnComponentBeginOverlapImpl(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PlaySoundFireAndForget(const FName& SoundName, EVirtualizationMode VirtualizationMode = EVirtualizationMode::PlayWhenSilent);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    FVector LaunchCharacterVelocity;
protected:
    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UBoxComponent* RootBoxComponent;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UAudioComponent* SoundComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TMap<FName, USoundBase*> SoundList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    USoundAttenuation* SoundAttenuation;
};
