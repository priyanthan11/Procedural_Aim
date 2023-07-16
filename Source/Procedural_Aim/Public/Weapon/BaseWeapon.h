// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class UStaticMeshComponent;

UCLASS()
class PROCEDURAL_AIM_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Sight;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optic", meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*> Optics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optic", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CurrentOptic;
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UStaticMeshComponent* GetCurrentOptic() const { return CurrentOptic; }
		 
};
