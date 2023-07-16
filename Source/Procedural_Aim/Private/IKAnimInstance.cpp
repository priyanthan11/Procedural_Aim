// Fill out your copyright notice in the Description page of Project Settings.


#include "IKAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "kismet/GameplayStatics.h"
#include "Procedural_Aim/Procedural_AimCharacter.h"
#include "Weapon/BaseWeapon.h"

UIKAnimInstance::UIKAnimInstance()
{
	AimAlpha = 0.f;
	bInterpAiming = false;
	bIsAiming = false;
}

void UIKAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Character = Cast<AProcedural_AimCharacter>(TryGetPawnOwner());
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), );
	//BaseWeapon = Character->GetBaseWeapon();
	
	
}

void UIKAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Character)
	{
		FTimerHandle TSetSightTransform;
		FTimerHandle TSetRelativeHandTransform;

		GetWorld()->GetTimerManager().SetTimer(TSetSightTransform, this, &UIKAnimInstance::setSightTransform, 0.3f, false);
		GetWorld()->GetTimerManager().SetTimer(TSetRelativeHandTransform, this, &UIKAnimInstance::setRelativeHandTransform, 0.3f, false);


		if (bInterpAiming)
		{
			interpAiming();
		}

	}
	
}

void UIKAnimInstance::setSightTransform()
{
	FTransform CamTransform = Character->GetFollowCamera()->GetComponentTransform();
	FTransform MeshTransform = Character->GetMesh()->GetComponentTransform();

	FTransform Relative = UKismetMathLibrary::MakeRelativeTransform(CamTransform, MeshTransform);
	
	FVector NewSightVector = Relative.GetLocation();
	FVector FowVector = Relative.GetRotation().GetForwardVector();
	FowVector *= 30.f;

	NewSightVector += FowVector;

	SightTransform.SetLocation(NewSightVector);
	SightTransform.SetRotation(Relative.Rotator().Quaternion());


}

void UIKAnimInstance::setRelativeHandTransform()
{
	if (Character->GetBaseWeapon()->GetCurrentOptic())
	{
		FTransform OpticSocketTransform = Character->GetBaseWeapon()->GetCurrentOptic()->GetSocketTransform(FName("S_Aim"));
		FTransform MeshTransform = Character->GetMesh()->GetSocketTransform(FName("hand_r"));

		RelativeHandTransform = UKismetMathLibrary::MakeRelativeTransform(OpticSocketTransform, MeshTransform);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get current optic"));
	}
	
}

void UIKAnimInstance::interpAiming()
{
	AimAlpha = UKismetMathLibrary::FInterpTo(AimAlpha, static_cast<float>(bIsAiming), GetWorld()->GetDeltaSeconds(),10.0f);
	if (AimAlpha >=1.0f || AimAlpha <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Alpha: %f"),AimAlpha);
		bInterpAiming = false;
	}
}

void UIKAnimInstance::SetAiming(bool IsAiming)
{
	if (bIsAiming != IsAiming)
	{
		bIsAiming = IsAiming;
		bInterpAiming = true;
	}
}
