// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class UNREALTUTORIAL_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		//UShapeComponent* Root; // every class has root component
		UShapeComponent* tBox;
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MyMesh;
	UPROPERTY(EditAnywhere)
		float SpeedScale;

	FVector PlayerStartingLocation = FVector(-1901.0f,-99.0f,235.0f);

	FVector BoxSize = FVector(1.5f,1.5f,1.5f);

	float RunningTime;

	// their should add UPrimitiveComponent* OverlappedComponent
	UFUNCTION()
		void TriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
		bool bMovesYAxis = true;

	UPROPERTY(EditAnywhere)
		bool bPursuit = false;
	UPROPERTY(EditAnywhere)
		bool bWander = false;
	UPROPERTY(EditAnywhere)
		bool bBye = false;

};
