// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankYouPlayerController.generated.h"

/**
 *
 */
UCLASS()
class TANKYOU_API ATankYouPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetPlayerEnabledState(bool bPlayerEnabled);

	void GameHasEnded(class AActor* EndGameFocus, bool bIsWinner);

	UFUNCTION(BluePrintImplementableEvent)
		void GameStart();	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameStartStatus")
		bool bGameStarted = false;

	//GamePause is also handled in Blueprint of this player controller class.

	UFUNCTION(BluePrintImplementableEvent)
		void GameOver(bool bWonGame);

protected:

	virtual void BeginPlay() override;

private:

	//UPROPERTY(EditAnywhere)
	//	float RestartDelay = 5;
	//FTimerHandle RestartTimer;
};
