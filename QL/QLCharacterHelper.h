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
#include "QLCharacterHelper.generated.h"

class UBoxComponent;

//------------------------------------------------------------
// This helper class serves more than one AQLCharacters at the same time!
//------------------------------------------------------------
UCLASS()
class QL_API AQLCharacterHelper : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLCharacterHelper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void PostInitializeComponents() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void RespawnCharacterRandomly(bool bSpawnAsBot);
protected:
    //------------------------------------------------------------
    //------------------------------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++Property")
    UBoxComponent* RespawnBoxComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    FVector RespawnExtent;
};
