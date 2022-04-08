// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TankYouGameState.generated.h"

/**
 * 
 */
UCLASS()
class TANKYOU_API ATankYouGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		float SoundValue = 40.f;
	
};
