#pragma once


#include"CoreMinimal.h"
#include"GameFramework/Actor.h"
#include"Weapon.generated.h"


// make sure we can acess blueprint
// 儲存角色 IK（Inverse Kinematics）屬性的結構
USTRUCT(BlueprintType)

struct FIKProperties 
{

    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        class UAnimSequence* AnimPose; // base pose

        // 瞄準偏移的浮點數值，預設為15.0f
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float AimOffset = 15.0f;

    // 自定義偏移的變換屬性，可在編輯器中進行編輯
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FTransform CustomOffsetTransform;
};



UCLASS(Abstract)

class UNREALTUTORIAL_API AWeapon : public AActor {
    GENERATED_BODY()
public:
    AWeapon();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        class USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        class USkeletalMeshComponent* Mesh;



    UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State")
        class ATrueFPSCharacter* CurrentOwner;


    // FIK
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configurations")
        FIKProperties IKProperties;

    // when we grab it 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configurations")
        FTransform PlacementTransform;

};