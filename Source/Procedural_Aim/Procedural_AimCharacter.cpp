// Copyright Epic Games, Inc. All Rights Reserved.

#include "Procedural_AimCharacter.h"
#include "Camera/CameraComponent.h"
#include "IKAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Weapon/BaseWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"


//////////////////////////////////////////////////////////////////////////
// AProcedural_AimCharacter

AProcedural_AimCharacter::AProcedural_AimCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	bIsAiming = false;

}

void AProcedural_AimCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	/* Spawn the defaultWeapon and attach to the mesh*/
	EquipWeapon(SpawnDefaultWeapon());

	AnimIK = Cast<UIKAnimInstance>(GetMesh()->GetAnimInstance());
	

}

void AProcedural_AimCharacter::CycleOptic()
{
	
	TP_Gun->SwitchOptic();
	AnimIK->CycledOptic();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProcedural_AimCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProcedural_AimCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProcedural_AimCharacter::Look);

	}

}

void AProcedural_AimCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AProcedural_AimCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AProcedural_AimCharacter::SetAiming(bool IsAiming)
{
	bIsAiming = IsAiming;
	if (AnimIK)
	{
		AnimIK->SetAiming(bIsAiming);
	}
	if (!HasAuthority())
	{
		Server_SetAiming(IsAiming);
	}
}

void AProcedural_AimCharacter::OnRep_IsAiming()
{
	if (AnimIK)
	{
		AnimIK->SetAiming(bIsAiming);
	}
}



bool AProcedural_AimCharacter::Server_SetAiming_Validate(bool IsAiming)
{
	return true;
}

void AProcedural_AimCharacter::Server_SetAiming_Implementation(bool IsAiming)
{
	SetAiming(IsAiming);
}

ABaseWeapon* AProcedural_AimCharacter::SpawnDefaultWeapon()
{
	if (DefaultWeaponClass)
	{
		// Spawn the TSub Class of Variable
		return TP_Gun = GetWorld()->SpawnActor<ABaseWeapon>(DefaultWeaponClass);
	}
	return nullptr;
}

void AProcedural_AimCharacter::EquipWeapon(ABaseWeapon* WeaponToEquipped)
{
	if (WeaponToEquipped)
	{
		// Get the hand socket
		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("GunHolder"));
		if (HandSocket)
		{
			// Attach the weapon to the hand socket RightHandSocket
			HandSocket->AttachActor(WeaponToEquipped, GetMesh());

		}
	}
}

void AProcedural_AimCharacter::Fire()
{
	if (TP_Gun == nullptr) return;
	
	TP_Gun->FireWeapon();
	PlayGunFireMontage();

}

void AProcedural_AimCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(AProcedural_AimCharacter, bIsAiming,COND_SkipOwner);

}

void AProcedural_AimCharacter::PlayGunFireMontage()
{
	if (FireMontage != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("FireMontage is Valid"));
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnimInstance is Valid"));
			AnimInstance->Montage_Play(FireMontage, 1.f);
			AnimInstance->Montage_JumpToSection(FName("StartFire"));
		}
	}
	
}
void AProcedural_AimCharacter::Reload()
{
	if (FireMontage != nullptr)
	{
		// Get the animation object for the character mesh
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(ReloadMontage, 1.f);
			AnimInstance->Montage_JumpToSection(FName("Reload"));
			
		}

	}
	if (AnimIK)
	{
		AnimIK->Reload();
	}
}



