// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class UStaticMeshComponent;
class USoundCue;
class UIKAnimInstance;

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
	USoundBase* FireSound;

	UPROPERTY(ReplicatedUsing = OnRep_OpticIndex)
	uint8 OpticIndex;

	

	UFUNCTION()
	virtual void OnRep_OpticIndex();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OpticIndex(uint8 NewIndex);
	bool Server_OpticIndex_Validate(uint8 NewIndex);
	//void Server_OpticIndex_Implementation(uint8 NewIndex);
	class UAnimInstance* Anim;
	UIKAnimInstance* IKAnim;
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	// Do when weapon Start to fire
	UFUNCTION()
	void FireWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void FireWeaponSound();

private:
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SwitchOptic();
	FORCEINLINE UStaticMeshComponent* GetCurrentOptic() const { return CurrentOptic; }
		
	
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
};
