// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class UStaticMeshComponent;
class USoundCue;

UCLASS()
class PROCEDURAL_AIM_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Sight;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optic", meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*> Optics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optic", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CurrentOptic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Properties", meta = (AllowPrivateAccess = "true"))
	USoundCue* FireSound;

	uint8 OpticIndex;
	
	class UIKAnimInstance* IKAnim;
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SwitchOptic();
	FORCEINLINE UStaticMeshComponent* GetCurrentOptic() const { return CurrentOptic; }
		
	FORCEINLINE USoundCue* GetFireSound() const { return FireSound; }
};
