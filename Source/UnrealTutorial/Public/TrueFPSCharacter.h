// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TrueFPSCharacter.generated.h"

UCLASS(Blueprintable)
class UNREALTUTORIAL_API ATrueFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATrueFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* Camera;

/////////////////////////////////////////////////////
protected:
	// The weapon classes spawned by defalut
	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
		TArray<TSubclassOf<class AWeapon>> DefaultWeapons;  //////////////////////


public:
		UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated, Category = "State")
		TArray<class AWeapon*> Weapons;

		UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentWeapon, Category = "State")
			class AWeapon* CurrentWeapon;


		UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State") // keep track of index when we switch weapon
			int32 CurrentIndex = 0;


		// 切換武器相關
		UFUNCTION(BlueprintCallable, Category = "Character")
			virtual void EquipWeapon(const int32 Index);


protected:
	UFUNCTION()
		virtual void OnRep_CurrentWeapon(const class AWeapon* OldWeapon);

/////////////////////////////////////////////////////
	UFUNCTION(Server, Reliable)
		void Server_SetCurrentWeapon(class AWeapon* Weapon);
	
	virtual void Server_SetCurrentWeapon_Implementation(class AWeapon* Weapon);






// 操作部分
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
		bool isSprinting;

	virtual void NextWeapon();
	virtual void LastWeapon();




	void MoveForward(const float Value);
	void MoveRight(const float Value);
	void LookUp(const float Value);
	void LookRight(const float Value);
	

};
