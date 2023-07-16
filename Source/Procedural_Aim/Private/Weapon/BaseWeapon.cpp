// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"
#include "IKAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Procedural_Aim/Procedural_AimCharacter.h"
// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Mesh for gun
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	
	/*Sight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sight"));
	RootComponent = WeaponMesh;
	Sight->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("S_Sight"));*/

	OpticIndex = 0;

	
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseWeapon::SwitchOptic()
{
	if (++OpticIndex >= Optics.Num())
	{
		OpticIndex = 0;
	}
	CurrentOptic = Optics[OpticIndex];
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

