#pragma once


#include"CoreMinimal.h"
#include"GameFramework/Actor.h"
#include"Weapon.generated.h"


// make sure we can acess blueprint
// �x�s���� IK�]Inverse Kinematics�^�ݩʪ����c
USTRUCT(BlueprintType)

struct FIKProperties 
{

    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        class UAnimSequence* AnimPose; // base pose

        // �˷ǰ������B�I�ƭȡA�w�]��15.0f
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float AimOffset = 15.0f;

    // �۩w�q�������ܴ��ݩʡA�i�b�s�边���i��s��
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