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
	bInterpRelativeHand = false;
	ReloadAlpha = 1.f;
}

void UIKAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Character = Cast<AProcedural_AimCharacter>(TryGetPawnOwner());
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), );
	//BaseWeapon = Character->GetBaseWeapon();
	

	if (Character)
	{
		FTimerHandle TSetSightTransform;
		FTimerHandle TSetRelativeHandTransform;

		GetWorld()->GetTimerManager().SetTimer(TSetSightTransform, this, &UIKAnimInstance::setSightTransform, 0.3f, true);
		GetWorld()->GetTimerManager().SetTimer(TSetRelativeHandTransform, this, &UIKAnimInstance::setRelativeHandTransform, 0.3f, true);

	}
	
}

void UIKAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Character) return;
	
	if (bInterpAiming)
	{
		interpAiming();
	}
	if (bInterpRelativeHand)
	{
		interpRelativehand();
	}
	setLeftHandIK();
}

void UIKAnimInstance::setSightTransform()
{
	FTransform CamTransform = Character->GetFollowCamera()->GetComponentTransform();
	FTransform MeshTransform = Character->GetMesh()->GetComponentTransform();

	SightTransform = UKismetMathLibrary::MakeRelativeTransform(CamTransform, MeshTransform);
	
	/*FVector NewSightVector = Relative.GetLocation();
	FVector FowVector = Relative.GetRotation().GetForwardVector();
	FowVector *= 30.f;

	NewSightVector += FowVector;*/

	SightTransform.SetLocation(SightTransform.GetLocation() + SightTransform.GetRotation().Vector() * 30.f);



}

void UIKAnimInstance::setRelativeHandTransform()
{
	if (Character->GetBaseWeapon()->GetCurrentOptic())
	{
		FTransform OpticSocketTransform = Character->GetBaseWeapon()->GetCurrentOptic()->GetSocketTransform(FName("S_Aim"));
		FTransform MeshTransform = Character->GetMesh()->GetSocketTransform(FName("hand_r"));

		RelativeHandTransform = UKismetMathLibrary::MakeRelativeTransform(OpticSocketTransform, MeshTransform);

	}

	
}

void UIKAnimInstance::setFinalHandTransform()
{
	if (Character->GetBaseWeapon()->GetCurrentOptic())
	{
		FTransform OpticSocketTransform = Character->GetBaseWeapon()->GetCurrentOptic()->GetSocketTransform(FName("S_Aim"));
		FTransform MeshTransform = Character->GetMesh()->GetSocketTransform(FName("hand_r"));

		FinalHandTransform = UKismetMathLibrary::MakeRelativeTransform(OpticSocketTransform, MeshTransform);

	}

}

void UIKAnimInstance::interpAiming()
{
	AimAlpha = UKismetMathLibrary::FInterpTo(AimAlpha, static_cast<float>(bIsAiming), GetWorld()->GetDeltaSeconds(),10.0f);
	if (AimAlpha >=1.0f || AimAlpha <= 0.0f)
	{
		bInterpAiming = false;
	}
}

void UIKAnimInstance::interpRelativehand()
{
	RelativeHandTransform = UKismetMathLibrary::TInterpTo(RelativeHandTransform, FinalHandTransform, GetWorld()->GetDeltaSeconds(), 10.0f);
	if (RelativeHandTransform.Equals(FinalHandTransform))
	{
		UE_LOG(LogTemp, Warning, TEXT("Equals"));
		bInterpRelativeHand = false;
	}
}

void UIKAnimInstance::setLeftHandIK()
{
	FTransform GunSocketTransform = Character->GetBaseWeapon()->GetWeaponMesh()->GetSocketTransform(FName("S_LeftHand"));
	FTransform MeshTransform = Character->GetMesh()->GetSocketTransform(FName("hand_r"));
	
	LeftHandTransform = UKismetMathLibrary::MakeRelativeTransform(GunSocketTransform, MeshTransform);


}

void UIKAnimInstance::SetAiming(bool IsAiming)
{
	if (bIsAiming != IsAiming)
	{
		bIsAiming = IsAiming;
		bInterpAiming = true;
	}
}

void UIKAnimInstance::CycledOptic()
{
	setFinalHandTransform();
	bInterpRelativeHand = true;
}

void UIKAnimInstance::Reload()
{
	if (ReloadAlpha == 1.0f)
	{
		ReloadAlpha = 0.0f;
	}
	else if(ReloadAlpha == 0.0f)
	{
		ReloadAlpha = 1.0f;
	}
}

void UIKAnimInstance::StopReload()
{
	ReloadAlpha = 1.0f;
}
