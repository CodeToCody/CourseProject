// Fill out your copyright notice in the Description page of Project Settings.

#include "TrueFPSCharacter.h"
#include "Weapon.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATrueFPSCharacter::ATrueFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// �C�@frame
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// �|�Q�A�������⪺entity�o�ۨ�
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetMesh(), FName("head"));

}

// Called when the game starts or when spawned
void ATrueFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	// server �M client ���ncheck
	if (HasAuthority()) {
		// ��class�̪��Z�����]�@�M
		for (const TSubclassOf<AWeapon>& WeaponClass : DefaultWeapons) {
			// �o��class���ūh���L
			if (!WeaponClass)continue;
			// �Z�������
			FActorSpawnParameters Params;
			Params.Owner = this; // ��e�� ATrueFPSCharacter entity

			// �ͦ��Z��
			// GetWorld() �O���o��e��H������
			// SpawnActor<AWeapon> �ͦ�AWeapon���O���禡
			// WeaponClass �O��template���ѼơA���w�n�ͦ������O
			// Params �]�w�F�֦��̡]Params.Owner = this�^
			AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, Params);
			const int32 Index = Weapons.Add(SpawnedWeapon);

			if (Index == CurrentIndex) {
				CurrentWeapon = SpawnedWeapon;
				OnRep_CurrentWeapon(nullptr);
			}

		}
	}
	
}


// Called to bind functionality to input
void ATrueFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ���a�����ʳ]�w�j��w���J�W��
	////////////////////////////////////////////////////////////////
	PlayerInputComponent->BindAction(FName("NextWeapon"),EInputEvent::IE_Pressed ,this, &ATrueFPSCharacter::NextWeapon);
	PlayerInputComponent->BindAction(FName("LastWeapon"),EInputEvent::IE_Pressed ,this, &ATrueFPSCharacter::LastWeapon);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ATrueFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ATrueFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ATrueFPSCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("LookRight"), this, &ATrueFPSCharacter::LookRight);
	////////////////////////////////////////////////////////////////

}








// �����Z�� //////////////////////////
void ATrueFPSCharacter::EquipWeapon(const int32 Index) {

	if (!Weapons.IsValidIndex(Index) || CurrentWeapon == Weapons[Index]) return;

	if (IsLocallyControlled()) {

		CurrentIndex = Index;

		const AWeapon* OldWeapon = CurrentWeapon;
		CurrentWeapon = Weapons[Index];
		OnRep_CurrentWeapon(OldWeapon);

	}
	else if (!HasAuthority()) {
		Server_SetCurrentWeapon(Weapons[Index]);
	}
}
void ATrueFPSCharacter::Server_SetCurrentWeapon_Implementation(AWeapon* NewWeapon) {

	const AWeapon* OldWeapon = CurrentWeapon; // "hi wo shi xiao ping ur, ni hao ya !"
	CurrentWeapon = NewWeapon;
	OnRep_CurrentWeapon(OldWeapon);
}

void ATrueFPSCharacter::NextWeapon() {
	const int32 Index = Weapons.IsValidIndex(CurrentIndex + 1) ? CurrentIndex + 1 : 0;
	EquipWeapon(Index);
}
void ATrueFPSCharacter::LastWeapon() {
	const int32 Index = Weapons.IsValidIndex(CurrentIndex - 1) ? CurrentIndex - 1 : Weapons.Num() - 1;
	EquipWeapon(Index);
}
////////////////////////////////////

void ATrueFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	// ���w Weapons �o�Ӱ}�C���ƻs���� COND_None�]�Y���i��S���ƻs����^
	DOREPLIFETIME_CONDITION(ATrueFPSCharacter, Weapons, COND_None);
	//
	DOREPLIFETIME_CONDITION(ATrueFPSCharacter, CurrentWeapon, COND_None);
}

////// ����j���]�w
void ATrueFPSCharacter::OnRep_CurrentWeapon(const AWeapon* OldWeapon) {
	if (CurrentWeapon) {

		// no current owner
		// attach to the character
		if (!CurrentWeapon->CurrentOwner) {
			// �@��}�l�j����V�O����
			// �o�̭n�ھڤ��P��mesh�վ�
			const FTransform& PlacementTransform = CurrentWeapon->PlacementTransform * GetMesh()->GetSocketTransform(FName("hand_r"));

				
			// right location
			CurrentWeapon->SetActorTransform(PlacementTransform,false,nullptr,ETeleportType::TeleportPhysics);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform,FName("hand_r"));
			// �n���L�ǳƦb�ù��W���
			// CurrentWeapon->Mesh->SetVisibility(true);
			CurrentWeapon->CurrentOwner = this;
		}
		// �@¾���ݪ���
		CurrentWeapon->Mesh->SetVisibility(true);
	}

	if (OldWeapon) {
		OldWeapon->Mesh->SetVisibility(false);

	}
}




// WASD ����
// �ƹ�����������
////////////////////////////////////////////////////////////////
void ATrueFPSCharacter::MoveForward(const float Value) {
	// ���o�ثe���������A������ Yaw (��������) �b��V
	const FVector& Direction = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X);
	// �N�e�i��V�M��J�ƭ����Ψ쨤�⪺����
	AddMovementInput(Direction, Value);
}

void ATrueFPSCharacter::MoveRight(const float Value) {
	const FVector& Direction = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void ATrueFPSCharacter::LookUp(const float Value) {
	// Pitch ������
	// ����e�ݦV�Wor�U
	AddControllerPitchInput(Value);
}

void ATrueFPSCharacter::LookRight(const float Value) {
	// Yaw ���訤
	// ����¶�����b����(�¦V���ܡA���������κu��)
	AddControllerYawInput(Value);
}
////////////////////////////////////////////////////////////////