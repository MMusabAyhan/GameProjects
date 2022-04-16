// Fill out your copyright notice in the Description page of Project Settings.


#include "TankYouGameMode.h"
#include "Tank.h"
#include "Turret.h"
#include "Kismet/GameplayStatics.h"
#include "TankYouPlayerController.h"
#include "GenericPlatform/ICursor.h"


void ATankYouGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ATankYouGameMode::HandleGameStart()
{
	//Setting some basics	

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	//Taken out for now since no need
	//TotalNumberofTurrets = GetTotalTowerCount();

	TankYouPlayerController = Cast<ATankYouPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (TankYouPlayerController)
	{
		TankYouPlayerController->CurrentMouseCursor = EMouseCursor::Crosshairs;

		//Letting the player controller know that the game started so that it can show the widget.

		TankYouPlayerController->GameStart();

		TankYouPlayerController->SetPlayerEnabledState(false);

		FTimerHandle PlayerEnableTimerHandle;

		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			TankYouPlayerController,
			&ATankYouPlayerController::SetPlayerEnabledState,
			true
		);

		GetWorldTimerManager().SetTimer(
			PlayerEnableTimerHandle,
			PlayerEnableTimerDelegate,
			StartDelay,
			false
		);

	}
}

// Taken out of game since a boss is added. No need to count all towers anymore. But in case we may want
//  to use it again in future, I leave it as comment.

//int32 ATankYouGameMode::GetTotalTowerCount()
//{
//	TArray<AActor*> TurretArray;
//	UGameplayStatics::GetAllActorsOfClass(this, ATurret::StaticClass(), TurretArray);
//
//	return TurretArray.Num();
//}

void ATankYouGameMode::ActorDied(AActor* ActorDied)
{
	if (ActorDied == Tank)
	{
		Tank->HandleDestruction();
		if (TankYouPlayerController)
		{
			TankYouPlayerController->SetPlayerEnabledState(false);
		}
		
		TankYouPlayerController->GameHasEnded(Tank, false);
		bGameOver = true;
	}

	else if (ATurret* DestroyedTurret = Cast<ATurret>(ActorDied))
	{
		DestroyedTurret->HandleDestruction();

		// Taken out of game since a boss is added. No need to count all towers anymore. But in case we may want
		//  to use it again in future, I leave it as comment.
		//TotalNumberofTurrets--; 

		// Game ends when Boss Turret dies
		if (DestroyedTurret->ActorHasTag(TEXT("Boss")))
		{
			TankYouPlayerController->GameHasEnded(Tank, true);
			bGameOver = true;
		}
	}
}

