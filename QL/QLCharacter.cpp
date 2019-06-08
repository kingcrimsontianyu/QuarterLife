//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#include "QLCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "QLWeaponPortalGun.h" // to be modified
#include "DrawDebugHelpers.h"
#include "QLWeaponManager.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLCharacter::AQLCharacter() :
CurrentWeapon(nullptr)
{
    Health = 25.0f;
    MaxHealth = 100.0f;
    Armor = 50.0f;
    MaxArmor = 100.0f;

    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    // set our turn rates for input
    BaseTurnRate = 45.0f;
    BaseLookUpRate = 45.0f;

    // Create a CameraComponent
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    // Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
    FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
    FirstPersonMesh->SetOnlyOwnerSee(true);
    FirstPersonMesh->SetupAttachment(FirstPersonCameraComponent);
    FirstPersonMesh->bCastDynamicShadow = false;
    FirstPersonMesh->CastShadow = false;
    FirstPersonMesh->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
    FirstPersonMesh->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

    // third person
    ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonMesh"));
    ThirdPersonMesh->SetupAttachment(GetCapsuleComponent());
    ThirdPersonMesh->bOwnerNoSee = true;
    ThirdPersonMesh->CastShadow = true;
    ThirdPersonMesh->bCastDynamicShadow = true;

    // Create a gun mesh component
    GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
    GunMesh->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
    GunMesh->bCastDynamicShadow = false;
    GunMesh->CastShadow = false;

    MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
    MuzzleLocation->SetupAttachment(GunMesh);
    MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

    // Default offset from the character location for projectiles to spawn
    GunOffset = FVector(100.0f, 0.0f, 10.0f);

    // Note: The ProjectileClass and the skeletal mesh/anim blueprints for FirstPersonMesh, GunMesh, and VR_Gun
    // are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // manager
    WeaponManager = NewObject<UQLWeaponManager>();
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
    GunMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

    //FirstPersonMesh->SetHiddenInGame(false, true);
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
//void AQLCharacter::Tick(float DeltaTime)
//{
//    Super::Tick(DeltaTime);
//}

//------------------------------------------------------------
// Called to bind functionality to input
//------------------------------------------------------------
void AQLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind jump events
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    // Bind fire event
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AQLCharacter::OnFire);
    PlayerInputComponent->BindAction("AltFire", IE_Pressed, this, &AQLCharacter::OnAltFire);

    // Bind movement events
    PlayerInputComponent->BindAxis("MoveForward", this, &AQLCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AQLCharacter::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        // add movement in that direction
        AddMovementInput(GetActorForwardVector(), Value);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        // add movement in that direction
        AddMovementInput(GetActorRightVector(), Value);
    }
}

//------------------------------------------------------------
// UE_LOG(LogTemp, Warning, TEXT("%s"), *CurrentWeapon->GetName());
//------------------------------------------------------------
void AQLCharacter::OnFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->Fire();

        //// try and play the sound if specified
        //if (FireSound)
        //{
        //    UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        //}

        //// try and play a firing animation if specified
        //if (FireAnimation)
        //{
        //    // Get the animation object for the arms mesh
        //    UAnimInstance* AnimInstance = FirstPersonMesh->GetAnimInstance();
        //    if (AnimInstance)
        //    {
        //        AnimInstance->Montage_Play(FireAnimation, 1.0f);
        //    }
        //}
    }
}

void AQLCharacter::OnAltFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->AltFire();
    }
}


//------------------------------------------------------------
//------------------------------------------------------------
float AQLCharacter::GetHealth() const
{
    return Health;
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLCharacter::GetMaxHealth() const
{
    return MaxHealth;
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLCharacter::GetArmor() const
{
    return Armor;
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLCharacter::GetMaxArmor() const
{
    return MaxArmor;
}

//------------------------------------------------------------
// Returns FirstPersonMesh subobject
//------------------------------------------------------------
USkeletalMeshComponent* AQLCharacter::GetFirstPersonMesh()
{
    return FirstPersonMesh;
}

//------------------------------------------------------------
// Returns FirstPersonCameraComponent subobject
//------------------------------------------------------------
UCameraComponent* AQLCharacter::GetFirstPersonCameraComponent() const
{
    return FirstPersonCameraComponent;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::EquipWeapon(AQLWeapon* Weapon)
{
    CurrentWeapon = Weapon;
}

//------------------------------------------------------------
//------------------------------------------------------------
FHitResult AQLCharacter::RayTraceFromCharacterPOV(float rayTraceRange)
{
    FCollisionQueryParams params(FName(TEXT("lineTrace")),
                                 true, // bTraceComplex
                                 this); // ignore actor
    params.bReturnPhysicalMaterial = false;

    FVector start = FirstPersonCameraComponent->GetComponentLocation();
    FVector end = FirstPersonCameraComponent->GetForwardVector() * rayTraceRange + start;

    FHitResult hitResult(ForceInit);
    GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Pawn, params);

    // useful properties
    // hitResult.bBlockingHit  // did ray hit something
    // hitResult.GetActor();   // the hit actor if there is one
    // hitResult.ImpactPoint;  // FVector
    // hitResult.ImpactNormal; // FVector

    // for debugging purpose
    // DrawDebugLine(GetWorld(), start, hitResult.ImpactPoint, FColor(255, 0, 0), true, -1, 0, 10);

    return hitResult;
}