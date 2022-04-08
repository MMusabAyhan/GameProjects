// Fill out your copyright notice in the Description page of Project Settings.


#include "TankYouPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

void ATankYouPlayerController::BeginPlay() 
{
	Super::BeginPlay();

}

void ATankYouPlayerController::SetPlayerEnabledState(bool bPlayerEnabled) 
{
	if (bPlayerEnabled)
	{
		GetPawn()->EnableInput(this);
	}
	else
	{
		GetPawn()->DisableInput(this);
	}
}

void ATankYouPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	// This will show win/lose screen widget!

	SetPlayerEnabledState(false);

	GameOver(bIsWinner);	
}