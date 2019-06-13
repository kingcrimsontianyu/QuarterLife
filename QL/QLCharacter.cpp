//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//------------------------------------------------------------

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
#include "QLUtility.h"
#include "QLPlayerController.h"
#include "Components/WidgetComponent.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLCharacter::AQLCharacter()
{
    Health = 100.0f;
    MaxHealth = 100.0f;
    Armor = 100.0f;
    MaxArmor = 100.0f;

    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    // set our turn rates for input
    BaseTurnRate = 45.0f;
    BaseLookUpRate = 45.0f;

    // Create a CameraComponent
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    //FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
    FirstPersonCameraComponent->RelativeLocation = FVector(-10.0f, 1.75f, 64.f);
    FirstPersonCameraComponent->bUsePawnControlRotation = true;
    FirstPersonCameraComponent->SetFieldOfView(100.0f);

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

    // Note: The ProjectileClass and the skeletal mesh/anim blueprints for FirstPersonMesh, GunMesh, and VR_Gun
    // are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // manager
    WeaponManager = CreateDefaultSubobject<UQLWeaponManager>(TEXT("WeaponManager"));
    WeaponManager->SetUser(this);

    // ui
    PlayerHealthArmorBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerHealthArmorBarWidgetComponent"));
    PlayerHealthArmorBarWidgetComponent->SetupAttachment(GetCapsuleComponent());
    PlayerHealthArmorBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLCharacter::BeginPlay()
{
    Super::BeginPlay();

    UpdateHealth();
    UpdateArmor();
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
    PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);

    // Bind fire event
    PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AQLCharacter::OnFire);
    PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &AQLCharacter::OnFireRelease);
    PlayerInputComponent->BindAction("AltFire", EInputEvent::IE_Pressed, this, &AQLCharacter::OnAltFire);
    PlayerInputComponent->BindAction("AltFire", EInputEvent::IE_Released, this, &AQLCharacter::OnAltFireRelease);

    PlayerInputComponent->BindAction("SwitchToRocketLauncher", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToRocketLauncher);
    PlayerInputComponent->BindAction("SwitchToLightningGun", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToLightningGun);
    PlayerInputComponent->BindAction("SwitchToRailGun", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToRailGun);
    PlayerInputComponent->BindAction("SwitchToPortalGun", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToPortalGun);

    PlayerInputComponent->BindAction("RestartLevel", EInputEvent::IE_Pressed, this, &AQLCharacter::OnRestartLevel);

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
    AQLWeapon* CurrentWeapon = WeaponManager->GetCurrentWeapon();
    if (CurrentWeapon)
    {
        CurrentWeapon->OnFire();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::OnFireRelease()
{
    AQLWeapon* CurrentWeapon = WeaponManager->GetCurrentWeapon();
    if (CurrentWeapon)
    {
        CurrentWeapon->OnFireRelease();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::OnAltFire()
{
    AQLWeapon* CurrentWeapon = WeaponManager->GetCurrentWeapon();
    if (CurrentWeapon)
    {
        CurrentWeapon->OnAltFire();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::OnAltFireRelease()
{
    AQLWeapon* CurrentWeapon = WeaponManager->GetCurrentWeapon();
    if (CurrentWeapon)
    {
        CurrentWeapon->OnAltFireRelease();
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
void AQLCharacter::AddWeapon(AQLWeapon* Weapon)
{
    WeaponManager->AddWeapon(Weapon);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SetCurrentWeapon(const FName& WeaponName)
{
    WeaponManager->SetCurrentWeapon(WeaponName);
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

//------------------------------------------------------------
//------------------------------------------------------------
float AQLCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (ActualDamage > 0.0f)
    {
        const float HealthAbsorbingFraction = 0.33f;
        const float ArmorAbsorbingFraction = 1.0f - HealthAbsorbingFraction;
        float HealthDamage = HealthAbsorbingFraction * ActualDamage;
        float ArmorDamage = ArmorAbsorbingFraction * ActualDamage;

        // calculate armor
        float RemainingAmor = Armor - ArmorDamage;
        if (RemainingAmor < 0.0f)
        {
            HealthDamage -= RemainingAmor;
            Armor = 0.0f;
        }
        else
        {
            Armor = RemainingAmor;
        }

        UpdateArmor();

        // calculate health
        float RemainingHealth = Health - HealthDamage;
        if (RemainingHealth < 0.0f)
        {
            Health = 0.0f;
        }
        else
        {
            Health = RemainingHealth;
        }

        UpdateHealth();

        if (Health <= 0.0f)
        {
            Die();
        }
    }

    return ActualDamage;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SetHealthArmorBarVisible(bool bFlag)
{
    if (PlayerHealthArmorBarWidgetComponent)
    {
        PlayerHealthArmorBarWidgetComponent->SetVisibility(bFlag);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::UpdateHealth()
{
    if (PlayerHealthArmorBarWidgetComponent)
    {
        UUserWidget* UUserWidgetResult = PlayerHealthArmorBarWidgetComponent->GetUserWidgetObject();
        if (UUserWidgetResult)
        {
            UQLPlayerHealthArmorBarUserWidget* Result = Cast<UQLPlayerHealthArmorBarUserWidget>(UUserWidgetResult);
            if (Result)
            {
                float HealthPercent = Health / MaxHealth;
                Result->UpdateHealthBar(HealthPercent);
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::UpdateArmor()
{
    if (PlayerHealthArmorBarWidgetComponent)
    {
        UUserWidget* UUserWidgetResult = PlayerHealthArmorBarWidgetComponent->GetUserWidgetObject();
        if (UUserWidgetResult)
        {
            UQLPlayerHealthArmorBarUserWidget* Result = Cast<UQLPlayerHealthArmorBarUserWidget>(UUserWidgetResult);
            if (Result)
            {
                float ArmorPercent = Armor / MaxArmor;
                Result->UpdateArmorBar(ArmorPercent);
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::OnRestartLevel()
{
    UGameplayStatics::OpenLevel(GetWorld(), "QLArena");
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLPlayerController* AQLCharacter::GetQLPlayerController()
{
    AController* MyController = GetController();
    if (!MyController)
    {
        return nullptr;
    }

    AQLPlayerController* MyQLPlayerController = Cast<AQLPlayerController>(MyController);
    if (!MyQLPlayerController)
    {
        return nullptr;
    }

    return MyQLPlayerController;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToRocketLauncher()
{
    SetCurrentWeapon(FName(TEXT("RocketLauncher")));
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToLightningGun()
{
    SetCurrentWeapon(FName(TEXT("LightningGun")));
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToRailGun()
{
    SetCurrentWeapon(FName(TEXT("RailGun")));
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToPortalGun()
{
    SetCurrentWeapon(FName(TEXT("PortalGun")));
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::Die()
{
    Destroy();
}
