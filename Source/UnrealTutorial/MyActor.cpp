// MyActor.cpp

#include "MyActor.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"

// �]�w�q�{��
AMyActor::AMyActor()
{
    // �N�� Actor �]�m���C�@�V���ե� Tick()�C�p�G���ݭn�A�i�H�����H�����ʯ�C
    PrimaryActorTick.bCanEverTick = true;

    // ��l�Ʈڲե�
    tBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
    tBox->SetGenerateOverlapEvents(true); // ���D�i��b�o��

    // �K�[�ʺA�ƥ�B�z��ơA������i�J��Ĳ�o
    tBox->OnComponentBeginOverlap.AddDynamic(this, &AMyActor::TriggerEnter);
    tBox->SetRelativeScale3D(BoxSize);

    // �i�D�����o�O�ڲե�
    RootComponent = tBox;

    // �Ы��R�A����ե�
    MyMesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
    // ���[��ڲե�
    MyMesh->AttachTo(RootComponent);

    // ��l�� SpeedScale
    SpeedScale = 0.0f;
}

// �C���}�l�ɩΥͦ��ɽե�
void AMyActor::BeginPlay()
{
    Super::BeginPlay();
}

// �C�@�V���|�ե�
void AMyActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector NewLocation = GetActorLocation();
    float DeltaMove = 0.0f; // 1 �� -1


    if (bWander) {
        // ��{���C�欰�A�o�̨ϥ��H�����ʪ��ܨ��޿�
        DeltaMove = FMath::FRandRange(-1.0f, 1.0f);
        NewLocation.Y += DeltaMove * SpeedScale;
        NewLocation.X += DeltaMove * SpeedScale;
    }else if (bPursuit) {
        
    }else if (bBye) {

    }else {

    }


    RunningTime += DeltaTime;
    SetActorLocation(NewLocation);
}

// ������i�J��Ĳ�o���ƥ�B�z���
void AMyActor::TriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // ���aĲ�o�ɱN���m�]�m����l��m
    if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
        OtherActor->SetActorLocation(PlayerStartingLocation);
}
