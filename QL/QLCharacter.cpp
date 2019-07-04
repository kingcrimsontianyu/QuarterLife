//------------------------------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")oc
//------------------------------------------------------------

#include "QLCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "QLWeapon.h"
#include "QLWeaponManager.h"
#include "QLAbility.h"
#include "QLAbilityManager.h"
#include "QLPowerupManager.h"
#include "QLUtility.h"
#include "QLPlayerController.h"
#include "Components/WidgetComponent.h"
#include "QLPlayerController.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "QLPowerup.h"
#include "QLUmgFirstPerson.h"
#include "QLUmgInventory.h"
#include "Components/AudioComponent.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLCharacter::AQLCharacter()
{
    Health = 150.0f;
    MaxHealth = 200.0f;
    Armor = 100.0f;
    MaxArmor = 150.0f;
    ProtectionMultiplier = 1.0f;

    bCanFireAndAltFire = true;
    bCanSwitchWeapon = true;

    // Set size for collision capsule
    // original value: 55.f, 96.0f
    GetCapsuleComponent()->InitCapsuleSize(30.0f, 85.0f);

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
    FirstPersonMesh->SetCollisionProfileName(TEXT("NoCollision"));
    FirstPersonMesh->CastShadow = false;
    FirstPersonMesh->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
    FirstPersonMesh->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

    // third person
    ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonMesh"));
    ThirdPersonMesh->SetupAttachment(GetCapsuleComponent());
    ThirdPersonMesh->SetCollisionProfileName(TEXT("NoCollision"));
    ThirdPersonMesh->bOwnerNoSee = true;
    ThirdPersonMesh->CastShadow = true;
    ThirdPersonMesh->bCastDynamicShadow = true;
    ThirdPersonMesh->bRenderCustomDepth = true;
    ThirdPersonMesh->CustomDepthStencilValue = 1;

    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

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

    if (WeaponManager)
    {
        WeaponManager->CreateAndAddAllWeapons(WeaponClassList);
    }

    if (AbilityManager)
    {
        AbilityManager->CreateAndAddAllAbilities(AbilityClassList);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    // manager
    // for some reason, if WeaponManager is instantiated in the constructor using CreateDefaultSubobject
    // it may still be nullptr after BeginPlay
    // to do: investigate
    WeaponManager = NewObject<UQLWeaponManager>(this);
    WeaponManager->SetUser(this);

    AbilityManager = NewObject<UQLAbilityManager>(this);
    AbilityManager->SetUser(this);

    PowerupManager = NewObject<UQLPowerupManager>(this);
    PowerupManager->SetUser(this);

    if (FirstPersonMesh)
    {
        UMaterialInterface* BasicMaterial = FirstPersonMesh->GetMaterial(0);
        DynamicMaterialFirstPersonMesh = FirstPersonMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BasicMaterial);
        FirstPersonMesh->SetMaterial(0, DynamicMaterialFirstPersonMesh.Get());
    }

    if (ThirdPersonMesh)
    {
        UMaterialInterface* BasicMaterial = ThirdPersonMesh->GetMaterial(0);
        DynamicMaterialThirdPersonMesh = ThirdPersonMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BasicMaterial);
        ThirdPersonMesh->SetMaterial(0, DynamicMaterialThirdPersonMesh.Get());
    }
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
    PlayerInputComponent->BindAction("SwitchToNailGun", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToNailGun);
    PlayerInputComponent->BindAction("SwitchToPortalGun", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToPortalGun);
    PlayerInputComponent->BindAction("SwitchToGrenadeLauncher", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToGrenadeLauncher);

    PlayerInputComponent->BindAction("RestartLevel", EInputEvent::IE_Pressed, this, &AQLCharacter::OnRestartLevel);

    PlayerInputComponent->BindAction("UseAbility", EInputEvent::IE_Pressed, this, &AQLCharacter::OnUseAbility);

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
    if (!bCanFireAndAltFire)
    {
        return;
    }

    if (!WeaponManager)
    {
        return;
    }

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
    if (!bCanFireAndAltFire)
    {
        return;
    }

    if (!WeaponManager)
    {
        return;
    }

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
    if (!bCanFireAndAltFire)
    {
        return;
    }

    if (!WeaponManager)
    {
        return;
    }

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
    if (!bCanFireAndAltFire)
    {
        return;
    }

    if (!WeaponManager)
    {
        return;
    }

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
void AQLCharacter::AddHealth(float Increment)
{
    float Temp = Health + Increment;

    if (Temp >= MaxHealth)
    {
        Health = MaxHealth;
    }
    else
    {
        Health = Temp;
    }

    UpdateHealth();
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
void AQLCharacter::AddArmor(float Increment)
{
    float Temp = Armor + Increment;

    if (Temp >= MaxArmor)
    {
        Armor = MaxArmor;
    }
    else
    {
        Armor = Temp;
    }

    UpdateArmor();
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
//------------------------------------------------------------
USkeletalMeshComponent* AQLCharacter::GetThirdPersonMesh()
{
    return ThirdPersonMesh;
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
    if (!WeaponManager)
    {
        return;
    }

    WeaponManager->AddWeapon(Weapon);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::AddAbility(AQLAbility* Ability)
{
    if (!AbilityManager)
    {
        return;
    }

    AbilityManager->AddAbility(Ability);
}

//------------------------------------------------------------
//------------------------------------------------------------
bool AQLCharacter::AddPowerup(AQLPowerup* Powerup)
{
    if (!PowerupManager)
    {
        return false;
    }

    return PowerupManager->AddPowerup(Powerup);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::RemovePowerup(AQLPowerup* Powerup)
{
    if (!PowerupManager)
    {
        return;
    }

    return PowerupManager->RemovePowerup(Powerup);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SetCurrentWeapon(const FName& QLName)
{
    if (!WeaponManager)
    {
        return;
    }

    WeaponManager->SetCurrentWeapon(QLName);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SetCurrentAbility(const FName& QLName)
{
    if (!AbilityManager)
    {
        return;
    }

    AbilityManager->SetCurrentAbility(QLName);
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
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // if the character is already dead, no further damage
    if (Health <= 0.0f)
    {
        return 0.0f;
    }

    // handle point damage
    if (DamageEvent.GetTypeID() == FPointDamageEvent::ClassID)
    {
        // adjust damage according to ProtectionMultiplier
        ActualDamage *= ProtectionMultiplier;
    }
    // handle radial damage
    else if (DamageEvent.GetTypeID() == FRadialDamageEvent::ClassID)
    {
        FRadialDamageEvent const* RadialDamageEventPtr = static_cast<FRadialDamageEvent const*>(&DamageEvent);

        FVector Temp = DamageCauser->GetActorLocation() - GetActorLocation();
        float Distance = Temp.Size();

        // linear interpolation
        // y = MaxDamage at x = 0
        // y = MinDamage at x = BlastRadius
        // y = ? at x = Distance
        // also adjust damage according to ProtectionMultiplier

        float MinDamage = ProtectionMultiplier * RadialDamageEventPtr->Params.MinimumDamage;
        float MaxDamage = ProtectionMultiplier * RadialDamageEventPtr->Params.BaseDamage;
        ActualDamage = (MinDamage - MaxDamage) / RadialDamageEventPtr->Params.OuterRadius * Distance + MaxDamage;

        if (ActualDamage < 0.0f)
        {
            ActualDamage = 0.0f;
        }
    }

    if (ActualDamage > 0.0f)
    {
        TakeDamageQuakeStyle(ActualDamage);
    }

    return ActualDamage;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::TakeDamageQuakeStyle(float ActualDamage)
{
    if (ActualDamage > 0.0f)
    {
        constexpr float HealthAbsorbingFraction = 1.0f / 3.0f;
        constexpr float ArmorAbsorbingFraction = 1.0f - HealthAbsorbingFraction;
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
    // update health bar
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

    // update umg
    AQLPlayerController* QLPlayerController = Cast<AQLPlayerController>(GetController());
    if (QLPlayerController)
    {
        QLPlayerController->GetUMG()->UpdateTextHealthValue(Health);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::UpdateArmor()
{
    // update armor bar
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

    // update umg
    AQLPlayerController* QLPlayerController = Cast<AQLPlayerController>(GetController());
    if (QLPlayerController)
    {
        QLPlayerController->GetUMG()->UpdateTextArmorValue(Armor);
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
    if (bCanSwitchWeapon)
    {
        SetCurrentWeapon(FName(TEXT("RocketLauncher")));
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToLightningGun()
{
    if (bCanSwitchWeapon)
    {
        SetCurrentWeapon(FName(TEXT("LightningGun")));
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToRailGun()
{
    if (bCanSwitchWeapon)
    {
        SetCurrentWeapon(FName(TEXT("RailGun")));
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToNailGun()
{
    if (bCanSwitchWeapon)
    {
        SetCurrentWeapon(FName(TEXT("NailGun")));
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToPortalGun()
{
    if (bCanSwitchWeapon)
    {
        SetCurrentWeapon(FName(TEXT("PortalGun")));
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToGrenadeLauncher()
{
    if (bCanSwitchWeapon)
    {
        SetCurrentWeapon(FName(TEXT("GrenadeLauncher")));
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::Die()
{
    UAnimSequence* Animation = PlayAnimationSequence("Death1");

    // get animation length
    float ActualAnimationLength = Animation->SequenceLength / Animation->RateScale;
    float DurationBeforeDestroyed = ActualAnimationLength + 3.0f;

    PlaySoundFireAndForget(FName(TEXT("Die")));

    // avoid blocking living characters
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    // destroy the character
    GetWorldTimerManager().SetTimer(DieTimerHandle,
        this,
        &AQLCharacter::OnDie,
        1.0f, // time interval in second
        false, // loop
        DurationBeforeDestroyed); // delay in second
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::OnDie()
{
    Destroy();
}

//------------------------------------------------------------
//------------------------------------------------------------
UAnimSequence* AQLCharacter::PlayAnimationSequence(const FName& AnimationSequenceName)
{
    UAnimSequence** Result = AnimationSequenceList.Find(AnimationSequenceName);
    if (!Result)
    {
        return nullptr;
    }

    UAnimSequence* Animation = *Result;
    if (!Animation)
    {
        return nullptr;
    }

    if (!ThirdPersonMesh)
    {
        return nullptr;
    }

    // prevent animation jitters
    UPawnMovementComponent* PawnMovementComponent = GetMovementComponent();
    UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(PawnMovementComponent);
    if (CharacterMovementComponent)
    {
        CharacterMovementComponent->bOrientRotationToMovement = false;
    }

    ThirdPersonMesh->PlayAnimation(Animation, false);
    return Animation;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SetDamageMultiplier(const float Value)
{
    WeaponManager->SetDamageMultiplier(Value);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SetProtectionMultiplier(const float Value)
{
    ProtectionMultiplier = Value;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::StartGlow()
{
    if (!PowerupManager)
    {
        return;
    }

    auto* TopPowerup = PowerupManager->GetTopPowerup();
    if (!TopPowerup)
    {
        return;
    }

    const FLinearColor Color = TopPowerup->GetGlowColor();
    FVector ColorVector(Color.R, Color.G, Color.B);

    // glow first person mesh
    if (FirstPersonMesh && DynamicMaterialFirstPersonMesh.IsValid())
    {
        DynamicMaterialFirstPersonMesh->SetScalarParameterValue("GlowEnabled", 1.0f);
        DynamicMaterialFirstPersonMesh->SetVectorParameterValue("GlowColor", ColorVector);
    }

    // glow third person mesh
    if (ThirdPersonMesh && DynamicMaterialThirdPersonMesh.IsValid())
    {
        DynamicMaterialThirdPersonMesh->SetScalarParameterValue("GlowEnabled", 1.0f);
        DynamicMaterialThirdPersonMesh->SetVectorParameterValue("GlowColor", ColorVector);
    }

    // glow weapon
    if (WeaponManager)
    {
        WeaponManager->StartGlowWeapon(Color);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::StopGlow()
{
    if (!PowerupManager)
    {
        return;
    }

    auto* TopPowerup = PowerupManager->GetTopPowerup();
    if (TopPowerup)
    {
        StartGlow();
        return;
    }

    // when no top powerup is left
    if (FirstPersonMesh && DynamicMaterialFirstPersonMesh.IsValid())
    {
        DynamicMaterialFirstPersonMesh->SetScalarParameterValue("GlowEnabled", 0.0f);
    }

    if (ThirdPersonMesh && DynamicMaterialThirdPersonMesh.IsValid())
    {
        DynamicMaterialThirdPersonMesh->SetScalarParameterValue("GlowEnabled", 0.0f);
    }

    if (WeaponManager)
    {
        WeaponManager->StopGlowWeapon();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SetCurrentWeaponVisibility(const bool bFlag)
{
    if (WeaponManager)
    {
        WeaponManager->SetCurrentWeaponVisibility(bFlag);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
bool AQLCharacter::IsAlive()
{
    if (Health > 0.0f)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::OnUseAbility()
{
    if (!AbilityManager)
    {
        return;
    }

    AQLAbility* CurrentAbility = AbilityManager->GetCurrentAbility();
    if (CurrentAbility)
    {
        CurrentAbility->OnUse();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::PlaySoundFireAndForget(const FName& SoundName)
{
    USoundBase** Result = SoundList.Find(SoundName);
    if (Result)
    {
        USoundBase* Sound = *Result;
        if (Sound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::PlaySound(const FName& SoundName)
{
    USoundBase** Result = SoundList.Find(SoundName);
    if (Result)
    {
        USoundBase* Sound = *Result;
        if (Sound)
        {
            SoundComponent->SetSound(Sound);
            SoundComponent->Play(0.0f);

            //// sound played using this function is fire and forget and does not travel with the actor
            //UGameplayStatics::PlaySoundAtLocation(this, Sound, User->GetActorLocation());
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::StopSound()
{
    if (SoundComponent)
    {
        if (SoundComponent->IsPlaying())
        {
            SoundComponent->Stop();
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SetFireEnabled(const bool bFlag)
{
    bCanFireAndAltFire = bFlag;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SetSwitchWeaponEnabled(const bool bFlag)
{
    bCanSwitchWeapon = bFlag;
}

//------------------------------------------------------------
//------------------------------------------------------------
bool AQLCharacter::HasWeapon(const FName& WeaponName)
{
    if (!WeaponManager)
    {
        return false;
    }

    return WeaponManager->HasWeapon(WeaponName);
}