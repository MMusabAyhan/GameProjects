// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER3D_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	float RestartDelay = 3;
	FTimerHandle RestartTimer;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDClass;

	UUserWidget* HUDWidget;
};
