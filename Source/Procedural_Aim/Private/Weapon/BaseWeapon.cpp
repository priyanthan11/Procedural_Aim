// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"
#include "IKAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Procedural_Aim/Procedural_AimCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimInstance.h"


// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Mesh for gun
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	
	OpticIndex = 0;
	
	
}

void ABaseWeapon::FireWeapon()
{
	FireWeaponSound();
	//PlayGunFireMontage();
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	UAnimInstance* baseAnimInstance = WeaponMesh->GetAnimInstance();
	if (WeaponMesh && WeaponMesh->GetAnimInstance())
	{
		IKAnim = Cast<UIKAnimInstance>(WeaponMesh->GetAnimInstance());
		if (IKAnim)
		{
			UE_LOG(LogTemp, Warning, TEXT("IKAnim Valid"))
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("IKAnim not Valid"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponMesh or Animation Instance is invalid"))
	}
}

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION(ABaseWeapon, OpticIndex, COND_SkipOwner);
}

void ABaseWeapon::FireWeaponSound()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}
}

void ABaseWeapon::SwitchOptic()
{
	if (++OpticIndex >= Optics.Num())
	{
		OpticIndex = 0;
	}
	CurrentOptic = Optics[OpticIndex];
	
	if (!HasAuthority())
	{
		Server_OpticIndex(OpticIndex);
	}
}
void ABaseWeapon::OnRep_OpticIndex()
{
	CurrentOptic = Optics[OpticIndex];
	if (IKAnim)
	{
		UE_LOG(LogTemp,Warning,TEXT("IKAnim Valid"))
		IKAnim->CycledOptic();
	}
	
	
}

bool ABaseWeapon::Server_OpticIndex_Validate(uint8 NewIndex)
{
	return true;
}

void ABaseWeapon::Server_OpticIndex_Implementation(uint8 NewIndex)
{
	OpticIndex = NewIndex;
	OnRep_OpticIndex();

}
// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

