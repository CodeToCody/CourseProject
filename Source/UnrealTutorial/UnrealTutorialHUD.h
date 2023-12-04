// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UnrealTutorialHUD.generated.h"

UCLASS()
class AUnrealTutorialHUD : public AHUD
{
	GENERATED_BODY()

public:
	AUnrealTutorialHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

