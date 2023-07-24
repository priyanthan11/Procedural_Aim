// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "IKAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURAL_AIM_API UIKAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
		UIKAnimInstance();
		virtual void NativeBeginPlay() override;
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;

		UPROPERTY(BlueprintReadOnly, Category = "Hand")
		class AProcedural_AimCharacter* Character;
		UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Hand")
		class ABaseWeapon* BaseWeapon;
		
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand")
			FTransform RelativeHandTransform;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand")
			FTransform SightTransform;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand")
			FTransform FinalHandTransform;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand")
			FTransform LeftHandTransform;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand")
			float AimAlpha;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand")
			float ReloadAlpha;

		bool bInterpAiming;
		
		bool bIsAiming;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand")
		bool bInterpRelativeHand;
		
protected:
	void setSightTransform();
	void setRelativeHandTransform();
	void setFinalHandTransform();
	void interpAiming();
	void interpRelativehand();
	void setLeftHandIK();


public:
	void SetAiming(bool IsAiming);

	void CycledOptic();
	
	void Reload();
	UFUNCTION(BlueprintCallable, Category = "Gun")
	void StopReload();
};
