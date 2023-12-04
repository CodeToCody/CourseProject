// Fill out your copyright notice in the Description page of Project Settings.

#include "TrueFPSCharacter.h"
#include "Weapon.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATrueFPSCharacter::ATrueFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// 每一frame
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// 會被你那隻角色的entity牽著走
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetMesh(), FName("head"));

}

// Called when the game starts or when spawned
void ATrueFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	// server 和 client 都要check
	if (HasAuthority()) {
		// 把class裡的武器都跑一遍
		for (const TSubclassOf<AWeapon>& WeaponClass : DefaultWeapons) {
			// 這個class為空則跳過
			if (!WeaponClass)continue;
			// 武器的函數
			FActorSpawnParameters Params;
			Params.Owner = this; // 當前的 ATrueFPSCharacter entity

			// 生成武器
			// GetWorld() 是取得當前對象的指標
			// SpawnActor<AWeapon> 生成AWeapon類別的函式
			// WeaponClass 是個template的參數，指定要生成的類別
			// Params 設定了擁有者（Params.Owner = this）
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

	// 玩家的移動設定綁到硬體輸入上面
	////////////////////////////////////////////////////////////////
	PlayerInputComponent->BindAction(FName("NextWeapon"),EInputEvent::IE_Pressed ,this, &ATrueFPSCharacter::NextWeapon);
	PlayerInputComponent->BindAction(FName("LastWeapon"),EInputEvent::IE_Pressed ,this, &ATrueFPSCharacter::LastWeapon);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ATrueFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ATrueFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ATrueFPSCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("LookRight"), this, &ATrueFPSCharacter::LookRight);
	////////////////////////////////////////////////////////////////

}








// 切換武器 //////////////////////////
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


	// 指定 Weapons 這個陣列的複製條件為 COND_None（即不進行特殊的複製條件）
	DOREPLIFETIME_CONDITION(ATrueFPSCharacter, Weapons, COND_None);
	//
	DOREPLIFETIME_CONDITION(ATrueFPSCharacter, CurrentWeapon, COND_None);
}

////// 那把槍的設定
void ATrueFPSCharacter::OnRep_CurrentWeapon(const AWeapon* OldWeapon) {
	if (CurrentWeapon) {

		// no current owner
		// attach to the character
		if (!CurrentWeapon->CurrentOwner) {
			// 一剛開始槍的方向是錯的
			// 這裡要根據不同的mesh調整
			const FTransform& PlacementTransform = CurrentWeapon->PlacementTransform * GetMesh()->GetSocketTransform(FName("hand_r"));

				
			// right location
			CurrentWeapon->SetActorTransform(PlacementTransform,false,nullptr,ETeleportType::TeleportPhysics);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform,FName("hand_r"));
			// 要讓他準備在螢幕上顯示
			// CurrentWeapon->Mesh->SetVisibility(true);
			CurrentWeapon->CurrentOwner = this;
		}
		// 一職都看的到
		CurrentWeapon->Mesh->SetVisibility(true);
	}

	if (OldWeapon) {
		OldWeapon->Mesh->SetVisibility(false);

	}
}




// WASD 移動
// 滑鼠的視角移動
////////////////////////////////////////////////////////////////
void ATrueFPSCharacter::MoveForward(const float Value) {
	// 取得目前控制器的旋轉，提取其 Yaw (水平旋轉) 軸方向
	const FVector& Direction = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X);
	// 將前進方向和輸入數值應用到角色的移動
	AddMovementInput(Direction, Value);
}

void ATrueFPSCharacter::MoveRight(const float Value) {
	const FVector& Direction = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void ATrueFPSCharacter::LookUp(const float Value) {
	// Pitch 俯仰角
	// 物體前端向上or下
	AddControllerPitchInput(Value);
}

void ATrueFPSCharacter::LookRight(const float Value) {
	// Yaw 偏航角
	// 物體繞垂直軸旋轉(朝向改變，但不俯仰或滾動)
	AddControllerYawInput(Value);
}
////////////////////////////////////////////////////////////////