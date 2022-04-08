// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankYouGameMode.generated.h"


UCLASS()
class TANKYOU_API ATankYouGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	void ActorDied(AActor* ActorDied);
	bool bGameOver = false;


protected:

	virtual void BeginPlay() override;

private:

	class ATank* Tank;
	class ATankYouPlayerController* TankYouPlayerController;

	float StartDelay = 4.f;

	UFUNCTION(BlueprintCallable)
		void HandleGameStart();

	int32 TotalNumberofTurrets;

	int32 GetTotalTowerCount();
};

