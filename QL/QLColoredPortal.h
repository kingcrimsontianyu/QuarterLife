//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "QLPortal.h"
#include "QLPortalEnum.h"
#include "QLColoredPortal.generated.h"

//----------------------------------------
// Blue / orange portal used for the portal gun
//----------------------------------------
UCLASS()
class QL_API AQLColoredPortal : public AQLPortal
{
	GENERATED_BODY()

public:
    AQLColoredPortal();

    //----------------------------------------
    // Given the type of portal and its spouse,
    // initialize self
    //----------------------------------------
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void Initialize(EPortalColor PortalColor, AQLPortal* Spouse);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void CleanUp();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetBlue();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetOrange();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetActive();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetInactive();

    //----------------------------------------
    //----------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    EPortalColor PortalColor;

    //------------------------------------------------------------
    // four important materials
    // --- 2 x default material: filled single-color ellipse
    // --- 2 x material: hollow single-color ellipse, to be filled by
    //     the render target at runtime
    //------------------------------------------------------------
};
