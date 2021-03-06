//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------

//------------------------------------------------------------
// The portal gun system consists of:
//
// Portal gun
//
// Portal
//     Scene capture component 2D
//         Project settings --> Engine rendering --> Lighting --> Support global clip plane for planar reflections
//
// Portal gun compatible actor
//    Any actor with a tag QLPortalCompatible is compatible with the portal gun
//------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/IntPoint.h"
#include "QLPortal.generated.h"

//------------------------------------------------------------
// Basic portal
//------------------------------------------------------------
UCLASS()
class QL_API AQLPortal : public AActor
{
	GENERATED_BODY()

public:
    //------------------------------------------------------------
	// Sets default values for this actor's properties
    //------------------------------------------------------------
	AQLPortal();

    //------------------------------------------------------------
	// Called every frame
    //------------------------------------------------------------
	virtual void Tick(float DeltaTime) override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetSpouse(AQLPortal* Spouse);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    UBoxComponent* GetBoxComponent();

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void Debug();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetCanUpdatePortalView(bool bFlag);

    UFUNCTION()
    void OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION()
    void OnOverlapEndForActor(AActor* OverlappedActor, AActor* OtherActor);

    void AddToRoll(AActor* GivenActor);

    void RemoveFromRoll(AActor* GivenActor);

    bool IsInMyRoll(AActor* GivenActor);

    //------------------------------------------------------------
    // Specify target material to which the render target feeds the view update per tick
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetPortalMaterialInstanceDynamic(UMaterialInstanceDynamic* PortalMaterialInstanceDynamicExt);

    //------------------------------------------------------------
    // Set the visibility of the mesh component (static mesh or skeletal mesh)
    // To be overridden by blueprint.
    // virtual void QLSetVisibility_Implementation(const bool bFlag); can be overriden in C++
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++Function")
    void QLSetVisibility(const bool bFlag);
    virtual void QLSetVisibility_Implementation(const bool bFlag);


protected:
    //------------------------------------------------------------
    // Called when the game starts or when spawned
    //------------------------------------------------------------
    virtual void BeginPlay() override;

    //------------------------------------------------------------
    //------------------------------------------------------------
    virtual void PostInitializeComponents() override;

    //------------------------------------------------------------
    // Update scene capture component
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void UpdateSCC();

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FVector ConvertDirectionToSpouseSpace(const FVector& OldDirection);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FVector ConvertLocationToSpouseSpace(const FVector& OldLocation);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FRotator ConvertRotationToSpouseSpace(const FRotator& OldRotator);

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    USceneCaptureComponent2D* SceneCaptureComponent;

    //------------------------------------------------------------
    // The game and editor crash if UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property") is added
    //------------------------------------------------------------
    UPROPERTY()
    UTextureRenderTarget2D* RenderTarget;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UBoxComponent* BoxComponent;

    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY()
    TWeakObjectPtr<UMaterialInstanceDynamic> PortalMaterialInstanceDynamic;

    //------------------------------------------------------------
    // The pairing portal
    //------------------------------------------------------------
    UPROPERTY()
    TWeakObjectPtr<AQLPortal> Spouse;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++Property")
    bool bCanUpdatePortalView;

    // Whether the portal can teleport players, or just provide views
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++Property")
    bool bCanTeleport;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++Property")
    FIntPoint PortalResolution;

    // The desired portal frame rate.
    // 0 (default) is to match the native frame rate.
    // Higher value is more resource demanding and affects performance more seriously.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++Property", meta = (ClampMin = "0.0", ClampMax = "360.0"))
    float PortalFrameRate;

    UPROPERTY()
    FTimerHandle UpdatePortalTimerHandle;

    UPROPERTY()
    float PortalUpdateInterval;

    UPROPERTY()
    TArray<AActor*> Roll;
};
