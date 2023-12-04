// MyActor.cpp

#include "MyActor.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"

// 設定默認值
AMyActor::AMyActor()
{
    // 將此 Actor 設置為每一幀都調用 Tick()。如果不需要，可以關閉以提高性能。
    PrimaryActorTick.bCanEverTick = true;

    // 初始化根組件
    tBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
    tBox->SetGenerateOverlapEvents(true); // 問題可能在這裡

    // 添加動態事件處理函數，當有物體進入時觸發
    tBox->OnComponentBeginOverlap.AddDynamic(this, &AMyActor::TriggerEnter);
    tBox->SetRelativeScale3D(BoxSize);

    // 告訴引擎這是根組件
    RootComponent = tBox;

    // 創建靜態網格組件
    MyMesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
    // 附加到根組件
    MyMesh->AttachTo(RootComponent);

    // 初始化 SpeedScale
    SpeedScale = 0.0f;
}

// 遊戲開始時或生成時調用
void AMyActor::BeginPlay()
{
    Super::BeginPlay();
}

// 每一幀都會調用
void AMyActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector NewLocation = GetActorLocation();
    float DeltaMove = 0.0f; // 1 到 -1


    if (bWander) {
        // 實現漫遊行為，這裡使用隨機移動的示例邏輯
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

// 當有物體進入時觸發的事件處理函數
void AMyActor::TriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 當玩家觸發時將其位置設置為初始位置
    if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
        OtherActor->SetActorLocation(PlayerStartingLocation);
}
