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
#include "QLCelestialBody.generated.h"

class UStaticMeshComponent;
//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API AQLCelestialBody : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLCelestialBody();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
    //------------------------------------------------------------
    // Whether the celestial body can rotate around its local axis (spin)
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    bool bCanRotate;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float RotationRateInDegree;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    FVector RotationAxis;

    //------------------------------------------------------------
    // Whether the celestial body can rotate around another actor (orbit)
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    bool bCanRevolve;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float RevolutionRateInDegree;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    FVector RevolutionAxisApproximate;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TWeakObjectPtr<AActor> RevolveAroundActor;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    bool bCanCastShadow;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    FLinearColor BaseColor;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UStaticMeshComponent* BasicStaticMesh;

    //------------------------------------------------------------
    // Points to the dynamic instanced material of BasicStaticMesh
    //------------------------------------------------------------
    UMaterialInstanceDynamic* DynamicBasicStaticMeshMaterial;

    FVector RevolutionAxisNorm;
};
