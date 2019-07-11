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
#include "QLRailBeam.generated.h"

class UParticleSystemComponent;

//------------------------------------------------------------
// In Blueprint, set these properties
// - beam component
//------------------------------------------------------------
UCLASS()
class QL_API AQLRailBeam : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLRailBeam();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called every frame
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    UParticleSystemComponent* GetBeamComponent();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UParticleSystemComponent* BeamComponent;
};
