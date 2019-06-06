//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

//----------------------------------------
// This section describes the implementation detail of a portal gun in QL.
//
// Portal gun
//
// Portal
//     Scene capture component 2D
//         Project settings --> Engine rendering --> Lighting --> Support global clip plane for planar reflections
//
// Portal gun compatible actor
//----------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QLPortal.generated.h"

//----------------------------------------
//----------------------------------------
UCLASS()
class QL_API AQLPortal : public AActor
{
	GENERATED_BODY()

public:
    //----------------------------------------
	// Sets default values for this actor's properties
    //----------------------------------------
	AQLPortal();

    //----------------------------------------
	// Called every frame
    //----------------------------------------
	virtual void Tick(float DeltaTime) override;

    //----------------------------------------
    //----------------------------------------
    virtual void PostInitializeComponents() override;

    //----------------------------------------
    //----------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetSpouse(AQLPortal* Spouse);

    //----------------------------------------
    //----------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    class USceneCaptureComponent2D* SceneCaptureComponent;

    //----------------------------------------
    //----------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UTextureRenderTarget2D* RenderTarget;

    //----------------------------------------
    //----------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    class UBoxComponent* BoxComponent;

    //----------------------------------------
    //----------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UStaticMeshComponent* FrameStaticMesh;

    //----------------------------------------
    //----------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UStaticMeshComponent* DisplayPlaneStaticMesh;

protected:
    //----------------------------------------
    // Called when the game starts or when spawned
    //----------------------------------------
    virtual void BeginPlay() override;

    //----------------------------------------
    // Update scene capture component
    //----------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void UpdateSCC();

    //----------------------------------------
    //----------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FVector ConvertDirectionToSpouseSpace(const FVector& OldDirection);

    //----------------------------------------
    //----------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FVector ConvertLocationToSpouseSpace(const FVector& OldLocation);

    //----------------------------------------
    //----------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FRotator ConvertRotationToSpouseSpace(const FRotator& OldRotator);

    AQLPortal* Spouse;

    UMaterialInstanceDynamic* DynamicDisplayPlaneMaterial;
};
