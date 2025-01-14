// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TestingGroundHUD.generated.h"

UCLASS()
class ATestingGroundHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATestingGroundHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	UPROPERTY(EditDefaultsOnly)
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

