// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "Procedural_AimCharacter.generated.h"

class UIKAnimInstance;

UCLASS(config=Game)
class AProcedural_AimCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class ABaseWeapon* TP_Gun;

	/*Set this in blueprints for the default weapon class*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseWeapon>DefaultWeaponClass;

	/* Montage for firing weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* FireMontage;

public:
	AProcedural_AimCharacter();
	

protected:

	UIKAnimInstance* AnimIK;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Aiming")
	void SetAiming(bool IsAiming);

	UPROPERTY(ReplicatedUsing = OnRep_IsAiming)
	bool bIsAiming;

	UFUNCTION()
	void OnRep_IsAiming();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAiming(bool IsAiming);
	bool Server_SetAiming_Validate(bool IsAiming);
	//void Server_SetAiming__Implementation(bool IsAiming);
	

	UFUNCTION(BlueprintCallable, Category = "Aiming")
	void CycleOptic();

	UFUNCTION(BlueprintCallable, Category = "Aiming")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Aiming")
	void PlayGunFireMontage();

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	ABaseWeapon* SpawnDefaultWeapon();
	void EquipWeapon(ABaseWeapon* WeaponToEquipped);
	UFUNCTION(BlueprintCallable, Category = "Gun")
	void Fire();
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class ABaseWeapon* GetBaseWeapon() const { return TP_Gun; }

	
};

